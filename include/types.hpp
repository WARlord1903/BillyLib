#ifndef TYPES_HPP
#define TYPES_HPP

#include "main.h"
#include <memory>


typedef std::shared_ptr<ADIEncoder> ADIEncoderPtr;
typedef std::shared_ptr<pros::IMU> IMUPtr;
typedef std::shared_ptr<pros::ADIDigitalOut> PneumaticsPtr;

#endif