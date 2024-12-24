#include "main.h"

float exponentialInputRemap(float input, float scale) {
    if (scale != 0) {
        return (powf(2.718, -(scale / 10)) + powf(2.718, (fabs(input) - 127) / 10) * (1 - powf(2.718, -(scale / 10)))) * input;
    }
    return input;
}

int sign(int num){
    return (num >= 0) ? 1 : -1;
}


void Chassis::arcade(int throttle, int angular){
    int deadzone = 5;

    throttle = (abs(throttle) <= deadzone) ? 0 : throttle;
    angular = (abs(angular) <= deadzone) ? 0 : angular;

    throttle =  exponentialInputRemap(throttle, 7.5);

    angular = exponentialInputRemap(angular, 7.5);

    left_motors.move(throttle + angular);
    right_motors.move(throttle - angular);
}

Chassis *chassis = new Chassis();


void initialize() {
    pros::lcd::initialize(); // initialize brain screen
}

void disabled() {}


/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
    // pros::lcd::register_btn1_cb(on_center_button);
    // pros::lcd::register_btn0_cb(on_left_button);
    // pros::lcd::register_btn2_cb(on_right_button);
}


/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void autonomous() {
}

pros::c::optical_rgb_s_t rgb_value;

// Arm motor

int armMotorCounter = 0;

bool nomoveflex = false;
bool nomovearm = false;
void autoMoveArm() {
    // Wall stake mech code
    // Should be ran inside a thread because it uses delay commands which can interrupt the main while true loop

    nomovearm = true;
    // Arm motor has a gear ratio of 5 therefore we need to multiply every angle by 5

    
    if (armMotorCounter == 0) {
        // Moves the arm to catch the ring
        armMotor.move_relative(39.5 * 5, 120);
    } else if (armMotorCounter == 1) {
        // Complicated steps to push it down for the next step
        nomoveflex = true;
        armMotor.move(-20);
        flexWheelIntake.move(60);
        pros::delay(20);
        armMotor.move(0);
        pros::delay(150);
        flexWheelIntake.move(0);
        nomoveflex = false;
    } else if (armMotorCounter == 2) {
        // Score the ring to the wall stake by making it swing 90 degrees 
        // Flex wheel intake should be slightly moved to prevent it from being stuck
        // flexWheelIntake.move_relative(30, 70);
        // armMotors.move_relative(75 * 5, 127);


        armMotor.move(127);
        pros::delay(600);
        armMotor.move(-127);
        pros::delay(800);
        armMotor.move(0);
        armMotorCounter = -1;

    } else {
        // Move it forwards and back - this does not require a degree
        armMotor.move(127);
        pros::delay(250);
        armMotor.move(-127);
        pros::delay(800);
        armMotor.move(0);
        armMotorCounter = -1; // due to the arm motor being increased, set it to -1 so -1 + 1 = 0
    }
    armMotorCounter++;
    nomovearm = false;
}

// Set a bunch of values
bool fwSwitch = 0; // Flex Wheel intake SWITCH
bool mogoValue = false; // Mogo Value switch
bool doinkerValue = false;
bool intakeValue = false;
int debugStatCount = 0;
bool goDetectRing = false;
bool yesdr = false;
bool usebrake = true;
bool manualarm = false;
int flexwheelstuckamt = 0;

bool hangbool = false;
void hang() {
    hangbool = !hangbool;
    if (hangbool) {
        armMotor.move_relative(160 * 5, 127);
        armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    } else {
        armMotor.move_relative(-160 * 5, 127);
    }
 
}

// Driver code
void opcontrol() {

    
    // Before the while true loop, set the arm motor to brake mode instead of coast to prevent slipping
    armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    

    // Main while true loop
    while (true) {
        // Get Values of the controller
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);


        int r1 = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1);
        int r2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
        int l1 = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1);
        int l2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);

        int a = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A);
        int b = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B);
        int x = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X);
        int y = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y);

        int downArrow = controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);
        int upArrow = controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP);

        // Should manual arm be on, arm motors can be moved using the joystick
        if (manualarm && !nomovearm) {armMotor.move(-rightY);}

        // Mogo mech code
		if (r1) {
			mogoValue = !mogoValue;
			mogo.set_value(mogoValue);
            controller.print(2, 0, "Mogo Piston %s", mogoValue ? "ON" : "OFF");
		}
        if (l1) {
            pros::Task armmotortask(autoMoveArm);
            controller.print(2, 0, "Arm moving...");
        }
        // Buttons
        if (a) {
            doinkerValue = !doinkerValue;
			doinker.set_value(doinkerValue);
            // controller.clear_line(2);
            controller.print(2, 0, "Doinker %s", doinkerValue ? "ON" : "OFF");

        }

        if (y) {
            usebrake = !usebrake;
            // // controller.clear_line(2);
            controller.print(2, 0, "Brake Mode: %s", usebrake ? "Hold" : "None");
  
        }
        if (x) {
            manualarm = !manualarm;
            controller.print(2, 0, "Manual Arm %s", doinkerValue ? "ON" : "OFF");

        }
        if (upArrow) {
            pros::Task armmotorhangtask(&hang);

        }
        // If disconnected, set the bot to be hold mode just in case
        // if (controller.is_connected()) {
        //     if (usebrake) {
        //         chassis.setBrakeHold();
        //     } else {
        //         chassis.setBrakeCoast();
        //     }

        // } else {
        //     chassis.setBrakeHold();
        // }

        // Function to move the robot

        chassis->arcade(leftY, rightX);


        // A code that does not work but it is intended to detect if the flex wheel intake/chain is "stuck" and therefore should be moved backwards a bit
        int fwmv = flexWheelIntake.get_actual_velocity();
        int fwmv2 = flexWheelIntake.get_target_velocity();
        if (!nomoveflex) {

       
            if (abs(fwmv) < 15 && fwmv2 > 0) {
                flexwheelstuckamt++;
                if (flexwheelstuckamt > 100) {
                    flexwheelstuckamt = 0;
                }
                else if (flexwheelstuckamt > 30) {
                    flexWheelIntake.move(127);
                } 
                else {
                    flexWheelIntake.move(   
                        r2 * -127 // Intake
                        + l2 * 127  // Outake

                        + downArrow * -30
                    );
                }
            } else {
                flexwheelstuckamt = 0;
            
                flexWheelIntake.move(   
                    r2 * -127 // Intake
                    + l2 * 127  // Outake

                    + downArrow * -30
                );
            }
        }

        // Delay to save resources. This also makes sure that code runs properly (e.g. 10 ticks = 100 milliseconds)
        pros::delay(10);



        // Debug
        debugStatCount++;
        if (debugStatCount >= 100) { // Runs every 1s
            // Print Batery
           controller.print(0, 0, "%i | %i | %i", fwmv, fwmv2, flexwheelstuckamt);

            // Reset
            debugStatCount = 0;

        }
    }
}
