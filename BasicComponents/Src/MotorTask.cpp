/**
  ******************************************************************************
  * @FileName			    MotorTask.cpp
  * @Description            Control motors
  * @author                 Chang Liu
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#include "includes.h"

int16_t GMP_PID_Output;

void ChassisMotor::Handle() {
    speedPid.PIDInfo.ref = targetAngle;
    speedPid.PIDInfo.fdb = RxMsgC6x0.rotateSpeed;
    speedPid.Calc();
    intensity = speedPid.PIDInfo.output;
}

void NormalMotor::Handle() {
    if (s_count == 1) {
        uint16_t thisAngle = RxMsgC6x0.angle;
        double thisSpeed = RxMsgC6x0.rotateSpeed * 6 / reductionRate;
        if (firstEnter) {
            lastRead = thisAngle;
            firstEnter = 0;
            return;
        }
        if (thisAngle <= lastRead) {
            if (lastRead - thisAngle > 4000)
                realAngle += (thisAngle + 8192 - lastRead) * 360 / 8192.0 / reductionRate;
            else
                realAngle -= (lastRead - thisAngle) * 360 / 8192.0 / reductionRate;
        } else {
            if (thisAngle - lastRead > 4000)
                realAngle -= (lastRead + 8192 - thisAngle) * 360 / 8192.0 / reductionRate;
            else
                realAngle += (thisAngle - lastRead) * 360 / 8192.0 / reductionRate;
        }
        anglePid.PIDInfo.fdb = realAngle;
        anglePid.PIDInfo.ref = targetAngle;
        anglePid.Calc();
        speedPid.PIDInfo.fdb = thisSpeed;
        speedPid.PIDInfo.ref = anglePid.PIDInfo.output;
        speedPid.Calc();
        intensity = speedPid.PIDInfo.output;
        s_count = 0;
        lastRead = thisAngle;
    } else {
        s_count++;
    }
}

void GMPitchMotor::Handle() {
    if (Gimbal::gimbal.feedbackType == encoder) {
        thisAngle = -(float) (GM_PITCH_ZERO - RxMsgC6x0.angle) * 360.0f / 8192.0f;
        thisSpeed = -IMU::BSP_IMU.GetData().wy;
    } else if (Gimbal::gimbal.feedbackType == imu) {
        thisAngle = IMU::BSP_IMU.GetData().pit;
        thisSpeed = -IMU::BSP_IMU.GetData().wy;
    }

    if (reductionRate >= 0) direction = 1;
    else direction = -1;

    if (firstEnter) {
        lastRead = thisAngle;
        realAngle = -(float) (GM_PITCH_ZERO - RxMsgC6x0.angle) * 360.0f / 8192.0f;
        NORMALIZE_ANGLE180(realAngle);
        firstEnter = 0;
        return;
    }
    if (thisAngle <= lastRead) {
        if (lastRead - thisAngle > 180)
            realAngle += (thisAngle + 360 - lastRead) * direction;
        else
            realAngle -= (lastRead - thisAngle) * direction;
    } else {
        if (thisAngle - lastRead > 180)
            realAngle -= (lastRead + 360 - thisAngle) * direction;
        else
            realAngle += (thisAngle - lastRead) * direction;
    }
    if (fabs(realAngle - targetAngle) < 10)
        reseted = 1;

    //	if(Chassis::chassis.chassisLock!=ChassisLockRCD){
    //		targetAngle=realAngle;
    //		ChassisLockRCD=Chassis::chassis.chassisLock;
    //	}
    INRANGE(targetAngle, realAngle + (GM_PITCH_ZERO - RxMsgC6x0.angle) * 360.0 / 8192.0 / reductionRate - 30.0f, realAngle + (GM_PITCH_ZERO - RxMsgC6x0.angle) * 360.0 / 8192.0 / reductionRate + 42.0f);

    if (reseted == 0) anglePid.PIDInfo.outputMax = 1.0;
    else anglePid.PIDInfo.outputMax = 10.0;

    lastRead = thisAngle;
    anglePid.PIDInfo.ref = targetAngle;
    anglePid.PIDInfo.fdb = realAngle;
    anglePid.Calc();
    speedPid.PIDInfo.ref = anglePid.PIDInfo.output;
    speedPid.PIDInfo.fdb = thisSpeed;
    speedPid.Calc();
    intensity = speedPid.PIDInfo.output;
	GMP_PID_Output = intensity;
}

void GMYawMotor::Handle() {
    if (Gimbal::gimbal.feedbackType == encoder) {
        thisAngle = -(float) (GM_YAW_ZERO - RxMsgC6x0.angle) * 360.0f / 8192.0f;
        thisSpeed = IMU::BSP_IMU.GetData().wz;
    } else if (Gimbal::gimbal.feedbackType == imu) {
        thisAngle = -IMU::BSP_IMU.GetData().yaw;
        thisSpeed = IMU::BSP_IMU.GetData().wz;
    }

    if (reductionRate >= 0) direction = 1;
    else direction = -1;

    if (firstEnter) {
        lastRead = thisAngle;
        realAngle = -(float) (GM_YAW_ZERO - RxMsgC6x0.angle) * 360.0f / 8192.0f;
        NORMALIZE_ANGLE180(realAngle);
        firstEnter = 0;
        return;
    }
    if (thisAngle <= lastRead) {
        if (lastRead - thisAngle > 180)
            realAngle += (thisAngle + 360 - lastRead) * direction;
        else
            realAngle -= (lastRead - thisAngle) * direction;
    } else {
        if (thisAngle - lastRead > 180)
            realAngle -= (lastRead + 360 - thisAngle) * direction;
        else
            realAngle += (thisAngle - lastRead) * direction;
    }
    if (fabs(realAngle - targetAngle) < 10)
        reseted = 1;

    //	if(Chassis::chassis.chassisLock!=ChassisLockRCD){
    //		targetAngle=realAngle;
    //		ChassisLockRCD=Chassis::chassis.chassisLock;
    //	}
#ifdef INFANTRY_3
    MINMAX(targetAngle, realAngle + (GM_PITCH_ZERO - RxMsgC6x0.angle) * 360.0 / 8192.0 / reductionRate - 30.0f, realAngle + (GM_PITCH_ZERO - RxMsgC6x0.angle) * 360.0 / 8192.0 / reductionRate + 42.0f);
#endif

    if (reseted == 0) anglePid.PIDInfo.outputMax = 1.0;
    else anglePid.PIDInfo.outputMax = 10.0;

    lastRead = thisAngle;
    anglePid.PIDInfo.ref = targetAngle;
    anglePid.PIDInfo.fdb = realAngle;
    anglePid.Calc();
    speedPid.PIDInfo.ref = anglePid.PIDInfo.output;
    speedPid.PIDInfo.fdb = thisSpeed;
    speedPid.Calc();
    intensity = speedPid.PIDInfo.output;
}

void Motor::motorInit() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 8; j++) {
            motors[i][j] = NULL;
        }
    }
}

void Motor::Reset(CanType_e can, uint16_t _RxID, double _reductionRate, double _kp, double _ki, double _kd, double _pMax, double _iMax,
                  double _dMax, double _max) {
    everRx = 0;
    RxMsgC6x0.angle = 0;
    reductionRate = _reductionRate;
    RxMsgC6x0.rotateSpeed = 0;
    RxMsgC6x0.moment = 0;
    targetAngle = 0;
    s_count = 0;
    reseted = 0;
    firstEnter = 1;
    lastRead = 0;
    realAngle = 0;
    intensity = 0;

    RxID = _RxID;
    TxID = 0x200;
    if (can == CAN_TYPE_1)
        Motor::motors[0][_RxID - 0x201] = this;
    if (can == CAN_TYPE_2)
        Motor::motors[1][_RxID - 0x201] = this;

    speedPid.Reset(_kp, _ki, _kd, _pMax, _iMax, _dMax, _max);
}

void Motor::StopReset() {
    everRx = 0;
    RxMsgC6x0.angle = 0;
    RxMsgC6x0.rotateSpeed = 0;
    RxMsgC6x0.moment = 0;
    targetAngle = 0;
    s_count = 0;
    reseted = 0;
    firstEnter = 1;
    lastRead = 0;
    realAngle = 0;
    intensity = 0;
}

void ChassisMotor::Reset(CanType_e can, uint16_t _RxID, double _reductionRate, double _kp, double _ki, double _kd, double _pMax, double _iMax,
                         double _dMax, double _max) {
    Motor::Reset(can, _RxID, _reductionRate, _kp, _ki, _kd, _pMax, _iMax, _dMax, _max);
}

void NormalMotor::Reset(CanType_e can, uint16_t _RxID, double _reductionRate, double _kp, double _ki, double _kd, double _pMax, double _iMax,
                        double _dMax, double _max,
                        double _anglekp, double _angleki, double _anglekd, double _anglepMax, double _angleiMax,
                        double _angledMax, double _anglemax) {
    Motor::Reset(can, _RxID, _reductionRate, _kp, _ki, _kd, _pMax, _iMax, _dMax, _max);
    anglePid.Reset(_anglekp, _angleki, _anglekd, _anglepMax, _angleiMax, _angledMax, _anglemax);
}

void GMPitchMotor::Reset(CanType_e can, uint16_t _RxID, double _reductionRate, double _kp, double _ki, double _kd, double _pMax, double _iMax,
                         double _dMax, double _max,
                         double _anglekp, double _angleki, double _anglekd, double _anglepMax, double _angleiMax,
                         double _angledMax, double _anglemax) {
    Motor::Reset(can, _RxID, _reductionRate, _kp, _ki, _kd, _pMax, _iMax, _dMax, _max);
    anglePid.Reset(_anglekp, _angleki, _anglekd, _anglepMax, _angleiMax, _angledMax, _anglemax);
    thisAngle = 0;
    thisSpeed = 0;
    direction = 1;
}

void GMYawMotor::Reset(CanType_e can, uint16_t _RxID, double _reductionRate, double _kp, double _ki, double _kd, double _pMax, double _iMax,
                       double _dMax, double _max,
                       double _anglekp, double _angleki, double _anglekd, double _anglepMax, double _angleiMax,
                       double _angledMax, double _anglemax) {
    Motor::Reset(can, _RxID, _reductionRate, _kp, _ki, _kd, _pMax, _iMax, _dMax, _max);
    anglePid.Reset(_anglekp, _angleki, _anglekd, _anglepMax, _angleiMax, _angledMax, _anglemax);
    thisAngle = 0;
    thisSpeed = 0;
    direction = 1;
}

Motor *Motor::motors[2][8];
