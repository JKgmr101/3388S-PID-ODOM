/**
 * \file main.h
 *
 * Contains common definitions and header files used throughout your PROS
 * project.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */


#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

/**
 * If defined, some commonly used enums will have preprocessor macros which give
 * a shorter, more convenient naming pattern. If this isn't desired, simply
 * comment the following line out.
 *
 * For instance, E_CONTROLLER_MASTER has a shorter name: CONTROLLER_MASTER.
 * E_CONTROLLER_MASTER is pedantically correct within the PROS styleguide, but
 * not convenient for most student programmers.
 */
#include "pros/adi.hpp"
#include "pros/motor_group.hpp"
#define PROS_USE_SIMPLE_NAMES

/**
 * If defined, C++ literals will be available for use. All literals are in the
 * pros::literals namespace.
 *
 * For instance, you can do `4_mtr = 50` to set motor 4's target velocity to 50
 */
#define PROS_USE_LITERALS

#include "api.h"
/**
 * You should add more #includes here
 */
//#include "okapi/api.hpp"

/**
 * If you find doing pros::Motor() to be tedious and you'd prefer just to do
 * Motor, you can use the namespace with the following commented out line.
 *
 * IMPORTANT: Only the okapi or pros namespace may be used, not both
 * concurrently! The okapi namespace will export all symbols inside the pros
 * namespace.
 */
// using namespace pros;
// using namespace pros::literals;
// using namespace okapi;
#ifndef FUNCTIONS
#define FUNCTIONS
class Chassis {
    public:
        void moveForwards(int inches);
        void turnToHeading(int degrees);

        void arcade(int throttle, int angular);
        void setBrakeHold();
        void setBrakeCoast();
        Chassis() {};
};

inline pros::MotorGroup left_motors({-20, -18, -17}, pros::MotorGearset::blue);
inline pros::MotorGroup right_motors({10, 8, 7}, pros::MotorGearset::blue);

inline pros::Motor flexWheelIntake(5, pros::v5::MotorGears::blue, pros::MotorEncoderUnits::degrees);

inline pros::Imu imu(12);

inline pros::Motor armMotor(4, pros::MotorGearset::rpm_600, pros::MotorEncoderUnits::degrees);

inline pros::adi::DigitalOut mogo('A');
inline pros::adi::DigitalOut doinker('B');

inline pros::Rotation verticalTracking(15);

inline pros::Rotation horizontalTracking(15);

inline pros::Controller controller(pros::E_CONTROLLER_MASTER);

void autoMoveArm();
#endif

#ifndef VARIABLES
#define VARIABLES

extern int armMotorCounter;

extern bool nomoveflex;
extern bool nomovearm;

// Set a bunch of values
extern bool fwSwitch; // Flex Wheel intake SWITCH
extern bool mogoValue; // Mogo Value switch
extern bool doinkerValue;
extern bool intakeValue;
extern int debugStatCount;
extern bool goDetectRing;
extern bool yesdr;
extern bool usebrake ;
extern bool manualarm;
extern int flexwheelstuckamt;
#endif
/**
 * Prototypes for the competition control tasks are redefined here to ensure
 * that they can be called from user code (i.e. calling autonomous from a
 * button press in opcontrol() for testing purposes).
 */
#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
 * You can add C++-only headers here
 */
//#include <iostream>
#endif

#endif  // _PROS_MAIN_H_
