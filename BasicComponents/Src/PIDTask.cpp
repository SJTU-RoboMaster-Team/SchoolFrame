/**
  ******************************************************************************
  * @FileName			    PIDTask.cpp
  * @Description            PID
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

void PID_Calc(PID_Regulator_t *pid) {
    pid->err[1] = pid->ref - pid->fdb;
    pid->componentKp = pid->err[1] * pid->kp;
    pid->errSum += pid->err[1];
    INRANGE(pid->errSum, -1 * pid->componentKiMax / pid->ki, pid->componentKiMax / pid->ki);
    pid->componentKi = pid->errSum * pid->ki;
    pid->componentKd = (pid->err[1] - pid->err[0]) * pid->kd;
    INRANGE(pid->componentKp, -1 * pid->componentKpMax, pid->componentKpMax);
    INRANGE(pid->componentKi, -1 * pid->componentKiMax, pid->componentKiMax);
    INRANGE(pid->componentKd, -1 * pid->componentKdMax, pid->componentKdMax);
    pid->output = pid->componentKp + pid->componentKi + pid->componentKd;
    INRANGE(pid->output, -1 * pid->outputMax, pid->outputMax);

    pid->err[0] = pid->err[1];
}

void PID_Reset(PID_Regulator_t *pid, double _kp, double _ki, double _kd, double _pMax, double _iMax, double _dMax,
               double _max) {
    pid->ref = 0;
    pid->fdb = 0;
    pid->err[0] = 0;
    pid->err[1] = 0;
    pid->componentKp = 0;
    pid->componentKi = 0;
    pid->componentKd = 0;
    pid->output = 0;
    pid->errSum = 0;
    pid->kp = _kp;
    pid->ki = _ki;
    pid->kd = _kd;
    pid->componentKpMax = _pMax;
    pid->componentKiMax = _iMax;
    pid->componentKdMax = _dMax;
    pid->outputMax = _max;
}
