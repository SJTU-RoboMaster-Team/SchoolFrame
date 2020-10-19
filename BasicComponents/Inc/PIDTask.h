/**
  ******************************************************************************
  * @FileName			    PIDTask.h
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

#ifndef __PID__H__
#define __PID__H__

#include "includes.h"

typedef struct PID_Regulator_t
{
	float ref;
	float fdb;
	float err[2];
	float errSum;
	float kp;
	float ki;
	float kd;
	float componentKp;
	float componentKi;
	float componentKd;
	float componentKpMax;
	float componentKiMax;
	float componentKdMax;
	float output;
	float outputMax;
} PID_Regulator_t;

void PID_Reset(PID_Regulator_t *pid, double _kp, double _ki, double _kd, double _pMax, double _iMax, double _dMax, double _max);
void PID_Calc(PID_Regulator_t *pid);

class PID
{
public:
	PID_Regulator_t PIDInfo;
	inline void Calc() { PID_Calc(&(this->PIDInfo)); }
	inline void Reset(double _kp, double _ki, double _kd, double _pMax, double _iMax, double _dMax, double _max)
	{
		PID_Reset(&(this->PIDInfo), _kp, _ki, _kd, _pMax, _iMax, _dMax, _max);
	}
};

#endif
