#ifndef ODOMCHASSIS_HPP
#define ODOMCHASSIS_HPP

#include "main.h"
#include "MathUtils/Point2D.hpp"
#include "MotionControllers/AbstractPIDController.hpp"
#include "MathUtils/PurePursuitPath.hpp"
#include "MotorGroup.hpp"
#include "types.hpp"


void odomLoop(void*);

struct Position{
    double x;
    double y;
    double orientation;
};

double linearModifier(double);
double exponentialModifier(double);
double squareModifier(double);
double cubicModifier(double);

class OdomChassis{
private:
    using joystick_modifier = double(*)(double);
public:
    OdomChassis(): impl{new OdomChassisImpl{}} {}
    OdomChassis(const OdomChassis &cpy): impl{cpy.impl} {}
    void operator=(const OdomChassis &cpy) { impl = cpy.impl; }

    void setPos(double ix, double iy, double iorientation) { impl->setPos(ix, iy, iorientation); }
    void setMotors(MotorGroupWrapper& ileft, MotorGroupWrapper& iright) { impl->setMotors(ileft, iright); }
    void setSensors(ADIEncoder ileft, ADIEncoder imid, ADIEncoder iright) { impl->setSensors(ileft, imid, iright); }
    void setSensors(ADIEncoder ileft, ADIEncoder imid, pros::IMU iimu) { impl->setSensors(ileft, imid, iimu); }
    void setKinematics(double maxVelocity, double maxAcceleration) { impl->kinematics.setMaxVelocity(maxVelocity); impl->kinematics.setMaxAcceleration(maxAcceleration); }
    void setDimensions(std::pair<AbstractMotor::gearset, double> igearRatio, double idiameter, double iwidth) { impl->setDimensions(igearRatio, idiameter, iwidth);  }
    void setOdomDimensions(std::pair<double, double> ileft, std::pair<double, double> imid, std::pair<double, double> iright) { impl->setOdomDimensions({ileft, imid, iright}); }
    void setGains(std::tuple<double, double, double> idrive, std::tuple<double, double, double> iturn, std::tuple<double, double, double> istraight) { impl->setGains(idrive, iturn, istraight); }
    void setDrive(joystick_modifier mod, double scale) { impl->mod = mod; impl->scale = scale; }

    void runOdom() { impl->updateTask = std::make_shared<pros::Task>(odomLoop, this, "Odom Update Task"); }
    void pauseOdom() { if(impl->updateTask) impl->updateTask->suspend(); }
    void resumeOdom() { if(impl->updateTask) impl->updateTask->resume(); }

    ADIEncoder& getLeftEncoder() { return impl->getLeftEncoder(); }
    ADIEncoder& getMidEncoder() { return impl->getMidEncoder(); }
    std::variant<ADIEncoderPtr, IMUPtr>& getAngular() { return impl->getAngular(); }
        
    double getLeftDiameter() { return impl->getLeftDiameter(); }
    double getLeftDistance() { return impl->getLeftDistance(); }
    double getRightDiameter() { return impl->getRightDiameter(); }
    double getRightDistance() { return impl->getRightDistance(); }
    double getMidDiameter() { return impl->getMidDiameter(); }
    double getMidDistance() { return impl->getMidDistance(); }

    double getMaxVelocity() { return impl->kinematics.getMaxVelocity(); }
    double getMaxAcceleration() { return impl->kinematics.getMaxAcceleration(); }

    auto& getDimensions() { return *(impl->dimensions); }

    void tank(double ileft, double iright) { impl->tank(ileft, iright); }
    void arcade(double ileft, double iright) { impl->arcade(ileft, iright); }

    MotorGroupWrapper& getLeft() { return *(impl->left_motors); }
    MotorGroupWrapper& getRight() { return *(impl->right_motors); }

    double getWheelBase() { return impl->dimensions->getWheelBase(); }

    void turnToPointAsync(Point2D p);
    void turnToPoint(Point2D p);
    void driveToPoint(Point2D p);
    void driveToPointAsync(Point2D p);
    
    void waitUntilSettled();

    double getX() { return impl->getX(); }
    double getY() { return impl->getY(); }
    double getOrientation() { return impl->getOrientation(); }
    Position getPosition() { return impl->getPosition(); }
    Point2D getPoint() { return {getPosition().x, getPosition().y}; }

    const std::tuple<double, double, double>& getDriveGains() { return impl->getDriveGains(); }
    const std::tuple<double, double, double>& getTurnGains() { return impl->getTurnGains(); }
    const std::tuple<double, double, double>& getStraightGains() { return impl->getStraightGains(); }

    void reset(double ix, double iy, double iorientation) { impl->reset(ix, iy, iorientation); }

    double getJoystickScale() { return impl->scale; }
    void setJoystickScale(double scale) { impl->scale = scale; }
    joystick_modifier getJoystickModifier() { return impl->mod; }
    void setJoystickModifier(joystick_modifier mod) { impl->mod = mod; } 

    void update() { impl->update(); }
private:
    void moveToPointAsync(Point2D p);
    void moveToPoint(Point2D p);
    struct OdomChassisImpl{
        OdomChassisImpl() = default;
        OdomChassisImpl(double ix, double iy, double iorientation): pos{ix, iy, iorientation} {}

        class ChassisDimensions{
        public:
            ChassisDimensions(AbstractMotor::gearset iinternalRatio, double iexternalRatio, double iwheelDiameter, double iwheelBase): internalRatio{iinternalRatio}, externalRatio{iexternalRatio}, wheelDiameter{iwheelDiameter}, wheelBase{iwheelBase} {}
            AbstractMotor::gearset getInternalGearRatio() { return internalRatio; }
            double getExternalGearRatio() { return externalRatio; }
            double getWheelDiameter() { return wheelDiameter; }
            double getWheelBase() { return wheelBase; }
        private:
            AbstractMotor::gearset internalRatio;
            double externalRatio;
            double wheelDiameter;
            double wheelBase;
        };

        class ChassisKinematics{
        public:
            ChassisKinematics() = default;
            ChassisKinematics(double imaxVel, double imaxAccel): maxVel{imaxVel}, maxAccel{imaxAccel} {}
            double getMaxVelocity() { return maxVel; }
            double getMaxAcceleration() { return maxAccel; }
            void setMaxVelocity(double iv) { maxVel = iv; }
            void setMaxAcceleration(double ia) { maxAccel = ia; }
        private:
            double maxVel;
            double maxAccel;
        };

        class TrackingWheels{
        public:
            TrackingWheels(std::pair<double, double> &ileft, std::pair<double, double> &imid, std::pair<double, double> &iright): dists{{{ileft.first, ileft.second}, {imid.first, imid.second}, {iright.first, iright.second}}} {}
            TrackingWheels(std::array<std::array<double, 2>, 3> &idists): dists{idists} {}
            double getLeftDistance() const { return dists[0][0]; }
            double getLeftDiameter() const { return dists[0][1]; }
            double getMidDistance() const { return dists[1][0]; }
            double getMidDiameter() const { return dists[1][1]; }
            double getRightDistance() const { return dists[2][0]; }
            double getRightDiameter() const { return dists[2][1]; }
        private:
            /* declared as follows:
                {
                    {left distance, left diameter},
                    {mid distance, mid diameter},
                    {right distance, right diameter}
                }
            */
            std::array<std::array<double, 2>, 3> dists;
        };

        class TrackingSensors{
        public:
            TrackingSensors(ADIEncoder ileft, ADIEncoder imid, ADIEncoder iangular): left{std::make_shared<ADIEncoder>(ileft)}, mid{std::make_shared<ADIEncoder>(imid)}, angular{std::make_shared<ADIEncoder>(iangular)} {}
            TrackingSensors(ADIEncoder ileft, ADIEncoder imid, pros::IMU iangular): left{std::make_shared<ADIEncoder>(ileft)}, mid{std::make_shared<ADIEncoder>(imid)}, angular{std::make_shared<pros::IMU>(iangular)} {}
            ADIEncoder& getLeftEncoder() { return *left; }
            ADIEncoder& getMidEncoder() { return *mid; }
            std::variant<ADIEncoderPtr, IMUPtr>& getAngular() { return angular; }
        private:
            ADIEncoderPtr left;
            ADIEncoderPtr mid;
            std::variant<ADIEncoderPtr, IMUPtr> angular;
        };

        ADIEncoder& getLeftEncoder(){ return sensors->getLeftEncoder(); }
        ADIEncoder& getMidEncoder(){ return sensors->getMidEncoder(); } 
        std::variant<ADIEncoderPtr, IMUPtr>& getAngular() { return sensors->getAngular(); }

        void tank(double, double);
        void arcade(double, double);

        double getX() { return pos.x; }
        double getY() { return pos.y; }
        double getOrientation() { return pos.orientation; }
        Position getPosition() { return pos; }

        void setPos(double ix, double iy, double iorientation) { pos = {ix, iy, iorientation}; }
        void reset(double ix, double iy, double iorientation) { if(std::holds_alternative<IMUPtr>(getAngular())) std::get<IMUPtr>(getAngular())->set_heading(iorientation * 180 / M_PI); pos = {ix, iy, iorientation}; }

        void update();

        const std::tuple<double, double, double>& getDriveGains() { return driveGains; }
        const std::tuple<double, double, double>& getTurnGains() { return turnGains; }
        const std::tuple<double, double, double>& getStraightGains() { return straightGains; }

        AbstractMotor::gearset getInternalGearRatio() { return dimensions->getInternalGearRatio(); }
        double getExternalGearRatio() { return dimensions->getExternalGearRatio(); }
        double getWheelDiameter() { return dimensions->getWheelDiameter(); }
        double getWheelBase() { return dimensions->getWheelBase(); }
        
        double getLeftDiameter() { return wheels->getLeftDiameter(); }
        double getLeftDistance() { return wheels->getLeftDistance(); }
        double getRightDiameter() { return wheels->getRightDiameter(); }
        double getRightDistance() { return wheels->getRightDistance(); }
        double getMidDiameter() { return wheels->getMidDiameter(); }
        double getMidDistance() { return wheels->getMidDistance(); }

        void setMotors(MotorGroupWrapper& ileft_motors, MotorGroupWrapper& iright_motors){ left_motors = &ileft_motors; right_motors = &iright_motors; left_motors->setEncoderUnits(AbstractMotor::encoderUnits::degrees); right_motors->setEncoderUnits(AbstractMotor::encoderUnits::degrees); }
        void setDimensions(std::pair<AbstractMotor::gearset, double> gearRatio, double idiameter, double iwidth) { dimensions = std::make_shared<ChassisDimensions>(gearRatio.first, gearRatio.second, idiameter, iwidth); }
        void setOdomDimensions(TrackingWheels iwheels) { wheels = std::make_shared<TrackingWheels>(iwheels); }
        void setSensors(ADIEncoder ileft, ADIEncoder imid, ADIEncoder iright) { sensors = std::make_shared<TrackingSensors>(ileft, imid, iright); }
        void setSensors(ADIEncoder ileft, ADIEncoder imid, pros::IMU iimu){ sensors = std::make_shared<TrackingSensors>(ileft, imid, iimu); }
        void setGains(std::tuple<double, double, double> idriveGains, std::tuple<double, double, double> iturnGains, std::tuple<double, double, double> istraightGains) { driveGains = idriveGains; turnGains = iturnGains; straightGains = istraightGains; }

        std::shared_ptr<ChassisDimensions> dimensions;
        std::shared_ptr<TrackingWheels> wheels;
        std::shared_ptr<TrackingSensors> sensors;
        ChassisKinematics kinematics;

        MotorGroupWrapper* left_motors;
        MotorGroupWrapper* right_motors;

        std::shared_ptr<pros::Task> updateTask;

        std::tuple<double, double, double> driveGains;
        std::tuple<double, double, double> turnGains;
        std::tuple<double, double, double> straightGains;

        Position pos;

        std::shared_ptr<AbstractPIDController> odom;

        double scale;
        joystick_modifier mod;
    };

    std::shared_ptr<OdomChassisImpl> impl;
};

#endif