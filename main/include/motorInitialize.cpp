#include "main.h"

pros::MotorGroup left_motors({-20, -18, -17}, pros::MotorGearset::blue);
pros::MotorGroup right_motors({10, 8, 9}, pros::MotorGearset::blue);

pros::Motor flexWheelIntake(5, pros::v5::MotorGears::blue, pros::MotorEncoderUnits::degrees);

pros::Imu imu(12);

pros::MotorGroup armMotors({-16, 4}, pros::MotorGearset::rpm_200, pros::MotorEncoderUnits::degrees);

pros::adi::DigitalOut mogo('A');
pros::adi::DigitalOut doinker('B');
pros::adi::DigitalOut intakePison('C');

pros::Rotation verticalTracking(15);

pros::Rotation horizontalTracking(15);
