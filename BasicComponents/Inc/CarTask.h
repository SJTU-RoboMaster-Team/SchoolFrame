/**
  ******************************************************************************
  * @FileName			    CarTask.h
  * @Description            This is a robot, not only a car
  * @author                 Chang Liu & Steve Young
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#ifndef __CAR_H__
#define __CAR_H__

#include "includes.h"

//#define BOARD_SLAVE
#define BOARD_MAIN

#ifdef BOARD_SLAVE
#define DOUBLE_BOARD_CAN1
#endif

class Car {
private:

public:
    static Car car;

    void Reset();

};

void CarInit();

void MainControlLoop();

void RemoteKeyMouseControlLoop();

void Count();

#endif
