#include "main.h"

int sign(int num){
    if (num == 0) return 0;
    return (num >= 0) ? 1 : -1;
}

float lateralInputRemap(float input, float scale) {
	if (scale != 0) {
		auto eq1 = [](float x, int a) {
            return powf(a, std::abs(x) - 127) * (std::abs(x)) *  sign(x);
        };


        return (127.0-10.0)/127.0 * (eq1(input, scale) * 127.0/(eq1(127, scale)));
    }
	return input;
}

float angularInputRemap(float input, float scale) {
    if (scale != 0) {
        return (powf(2.718, -(scale / 10)) + powf(2.718, (fabs(input) - 127) / 10) * (1 - powf(2.718, -(scale / 10)))) * input;
    }
    return input;
}


void Chassis::arcade(int throttle, int angular){
    int deadzone = 5;

    throttle = (abs(throttle) <= deadzone) ? 0 : throttle;
    angular = (abs(angular) <= deadzone) ? 0 : angular;

    throttle =  lateralInputRemap(throttle, 1.021);

    angular = angularInputRemap(angular, 7.5);

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
        armMotor.move_absolute(43 * 5, 120);

    } else if (armMotorCounter == 1) {
        // Complicated steps to push it down for the next step
        nomoveflex = true;
        armMotor.move(-20);
        flexWheelIntake.move(50);
        pros::delay(20);
        armMotor.move(0);
        pros::delay(100);
        flexWheelIntake.move(5);
        nomoveflex = false;
    } else if (armMotorCounter == 2) {
        armMotor.move_absolute(170*5, 1000);
        pros::delay(700);
        armMotor.move_absolute(0, 1000);
        armMotorCounter = -1;

    }
    armMotorCounter++;
    nomovearm = false;
}