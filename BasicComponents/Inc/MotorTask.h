/**
  ******************************************************************************
  * @FileName			    MotorTask.h
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

#ifndef __MOTORTASK_H__
#define __MOTORTASK_H__

#include "includes.h"


enum GMFeedbackType_e {
    encoder,
    imu
};

typedef struct {
    int16_t angle;
    int16_t rotateSpeed;
    int16_t moment;
} ESCC6x0RxMsg_t;

class Motor {
public:
    static Motor *motors[2][8];

    uint8_t everRx;

    double targetAngle;

    static void motorInit();

    virtual void
    Reset(CanType_e can, uint16_t _RxID, double _reductionRate, double _kp, double _ki, double _kd, double _pMax,
          double _iMax, double _dMax,
          double _max,
          double _anglekp, double _angleki, double _anglekd, double _anglepMax, double _angleiMax, double _angledMax,
          double _anglemax) {}

    void
    Reset(CanType_e can, uint16_t _RxID, double _reductionRate, double _kp, double _ki, double _kd, double _pMax,
          double _iMax, double _dMax,
          double _max);

    void StopReset();

    inline ESCC6x0RxMsg_t GetRxMsgC6x0() { return RxMsgC6x0; }

    inline void SetRxMsgC6x0(int16_t _angle, int16_t _rotateSpeed, int16_t _moment) {
        RxMsgC6x0.angle = _angle;
        RxMsgC6x0.rotateSpeed = _rotateSpeed;
        RxMsgC6x0.moment = _moment;
    }

    inline PID GetSpeedPID() { return speedPid; }

    inline int16_t GetIntensity() const { return intensity; }

    inline double GetRealAngle() const {return realAngle;}

    virtual void Handle() {}

protected:
    PID speedPid;
    PID anglePid;
    uint16_t TxID;
    uint16_t RxID;
    int16_t intensity;
    ESCC6x0RxMsg_t RxMsgC6x0;
    uint8_t firstEnter;
    uint8_t s_count;
    uint8_t reseted;
    double reductionRate;

    double lastRead;
    double realAngle;

};

class ChassisMotor : public Motor {
public:
    ChassisMotor() : Motor() {}

    void Handle();

    void
    Reset(CanType_e can, uint16_t _RxID, double _reductionRate, double _kp, double _ki, double _kd, double _pMax,
          double _iMax, double _dMax,
          double _max);
};

class NormalMotor : public Motor {
public:
    void
    Reset(CanType_e can, uint16_t _RxID, double _reductionRate, double _kp, double _ki, double _kd, double _pMax,
          double _iMax, double _dMax,
          double _max,
          double _anglekp, double _angleki, double _anglekd, double _anglepMax, double _angleiMax, double _angledMax,
          double _anglemax);

    void Handle();

    NormalMotor() : Motor() {}
};

class GMPitchMotor : public Motor {
public:
    void
    Reset(CanType_e can, uint16_t _RxID, double _reductionRate, double _kp, double _ki, double _kd, double _pMax,
          double _iMax, double _dMax,
          double _max,
          double _anglekp, double _angleki, double _anglekd, double _anglepMax, double _angleiMax, double _angledMax,
          double _anglemax);

    void Handle();

private:
    uint8_t direction;
    double thisSpeed;
    double thisAngle;
};

class GMYawMotor : public Motor {
public:
    void
    Reset(CanType_e can, uint16_t _RxID, double _reductionRate, double _kp, double _ki, double _kd, double _pMax,
          double _iMax, double _dMax,
          double _max,
          double _anglekp, double _angleki, double _anglekd, double _anglepMax, double _angleiMax, double _angledMax,
          double _anglemax);

    void Handle();

private:
    uint8_t direction;
    double thisSpeed;
    double thisAngle;
};

#endif
