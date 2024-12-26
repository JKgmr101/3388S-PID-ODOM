#include "main.h"
#include "pros/motors.h"

int sign(int num){
    if (num == 0) return 0;
    return (num >= 0) ? 1 : -1;
}

float inputRemapLemlib(float input, float scale) {
	if (scale != 0) {
		auto eq1 = [](float x, int a) {
            return powf(a, std::abs(x) - 127) * (std::abs(x)) *  sign(x);
        };


        return (127.0-10.0)/127.0 * (eq1(input, scale) * 127.0/(eq1(127, scale))) + 5*sign(input);
    }
	return input;
}

float gauthamInputRemap(float input, float scale) {
    if (scale != 0) {
        return (powf(2.718, -(scale / 10)) + powf(2.718, (fabs(input) - 127) / 10) * (1 - powf(2.718, -(scale / 10)))) * input;
    }
    return input;
}


void Chassis::arcade(int throttle, int angular){
    int deadzone = 5;

    throttle = (abs(throttle) <= deadzone) ? 0 : throttle;
    angular = (abs(angular) <= deadzone) ? 0 : angular;

    throttle =  inputRemapLemlib(throttle, 1.02);

    angular = gauthamInputRemap(angular, 7.5);

    left_motors.move(throttle + angular);
    right_motors.move(throttle - angular);
}



// Arm motor

int armMotorCounter = 0;

bool nomoveflex = false;
bool nomovearm = false;

void autoMoveArm() {
    // Wall stake mech code
    // Should be ran inside a thread because it uses delay commands which can interrupt the main while true loop
    armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    nomovearm = true;
    // Arm motor has a gear ratio of 5 therefore we need to multiply every angle by 5

    
    if (armMotorCounter == 0) {
        // Moves the arm to catch the ring
        armMotor.move_relative(45 * 5, 120);

    } else if (armMotorCounter == 1) {
        // Complicated steps to push it down for the next step
        nomoveflex = true;
        armMotor.move(-20);
        flexWheelIntake.move(20);
        pros::delay(20);
        armMotor.move(0);
        pros::delay(100);
        flexWheelIntake.move(5);
        nomoveflex = false;
    } else if (armMotorCounter == 2) {
        // Score the ring to the wall stake by making it swing 90 degrees 
        // Flex wheel intake should be slightly moved to prevent it from being stuck
        // flexWheelIntake.move_relative(30, 70);
        // armMotors.move_relative(75 * 5, 127);


        armMotor.move(127);
        pros::delay(600);
        armMotor.move(-127);
        pros::delay(500);
        armMotor.move(0);
        armMotorCounter = -1;

    }
    armMotorCounter++;
    nomovearm = false;
}