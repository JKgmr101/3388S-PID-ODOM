#include "main.h"

lateralPID latPID(10, 0, 2);
angularPID angPID(2, 0, 5);

float Chassis::compute(float destination, float currentDistance){
    float error = destination - currentDistance;

    float proportional = latPID.getP() * error;

    float integral = latPID.getI() * error;

    float derivative = latPID.getD() * (error - latPID.getPrevError());

    float output = proportional + integral + derivative;

    latPID.setPrevError(error);

    return output;
}

float 