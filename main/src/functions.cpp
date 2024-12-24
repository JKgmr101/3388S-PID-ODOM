#include "main.h"

int sign(int num){
    return (num >= 0) ? 1 : -1;
}

float lateralInputRemap(float input, float scale) {
	if (scale != 0) {
		auto eq1 = [](float x, int a) {
            return powf(a, std::abs(x) - 127) * (std::abs(x)) *  sign(x);
        };


        return (127.0-10.0)/127.0 * (eq1(input, scale) * 127.0/(eq1(127, scale))) + 10*sign(input);
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