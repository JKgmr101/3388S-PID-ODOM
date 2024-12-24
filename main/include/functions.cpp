#include "main.h"

float exponentialInputRemap(float input, float scale) {
    if (scale != 0) {
        return (powf(2.718, -(scale / 10)) + powf(2.718, (fabs(input) - 127) / 10) * (1 - powf(2.718, -(scale / 10)))) * input;
    }
    return input;
}


void Chassis::arcade(int throttle, int angular){
    int deadzone = 5;

    throttle = (throttle <= deadzone) ? 0 : throttle;
    angular = (angular <= deadzone) ? 0 : angular;

    throttle = exponentialInputRemap(throttle, 2.5);

    angular = exponentialInputRemap(angular, 1.4);

    left_motors.move(throttle + angular);
    right_motors.move(throttle - angular);
}