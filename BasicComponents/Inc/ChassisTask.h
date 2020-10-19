/**
  ******************************************************************************
  * @FileName			    ChassisTask.h
  * @Description            Control the chassis motors and limit the power of them
  *                         Follow the gimbal or twist or spin
  * @author                 Steve Young
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "includes.h"

#define CHASSIS_SPEED_K            0.85f
#define ROTATE_SPEED_K                0.07f

#define POWER_LIMITATION_DEBUG


struct PowerLimitation_t {
    double kIntensityToMoment;
    double PowerMax;
    double rotateSpeedRate;
    double CMFLRotateSpeed;
    double CMFRRotateSpeed;
    double CMBLRotateSpeed;
    double CMBRRotateSpeed;
};

class Chassis {
public:
    static Chassis chassis;
    ChassisMotor CMFL, CMFR, CMBL, CMBR;
    uint8_t twistState;
    uint8_t lock;

    void Reset();

    void Handle();

    void SetVelocity(int16_t _forwardBackVelocity, int16_t _leftRightVelocity, int16_t _rotateVelocity = 0);

    void AddVelocity(int16_t accFB, int16_t accLR);

    //inline void SetChassisLock(uint8_t value) { chassisLock = value; }

    //inline uint8_t GetChassisLock() { return chassisLock; }

    void PowerLimitation();


private:
    float forwardBackVelocity;
    float leftRightVelocity;
    float rotateVelocity;
    PID rotate;
    int8_t twistGapAngle;
    int16_t followCenter;
    uint8_t changeForwardBack;
    uint8_t changeForwardBackRCD;
    uint8_t changeForwardBackStep;
    float angleToCenter;

    PowerLimitation_t powerLimPara;

    void ControlRotate();

    void ChassisDataDecoding();

    void TwistSpin();
};

#endif
