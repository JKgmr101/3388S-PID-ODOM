#include "main.h"

Chassis chassis;

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

// Driver code
void opcontrol() {

    
    // Before the while true loop, set the arm motor to brake mode instead of coast to prevent slipping
    armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    

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

        chassis.arcade(leftY, rightX);


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
