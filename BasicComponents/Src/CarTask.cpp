/**
  ******************************************************************************
  * @FileName			    CarTask.cpp
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

#include <CarTask.h>

#include "includes.h"

void CarInit(void) {
    Car::car.Reset();
}

void MainControlLoop(void) {
    if (Remote::remote.GetWorkState() > 0) {
        Chassis::chassis.Handle();
        Additional::additional.Handle();
//        Gimbal::gimbal.Handle();
//        Shoot::shoot.Handle();
//        Gate::gate.Handle();
    }
    CAN::can1.TxHandle();
    CAN::can2.TxHandle();
    IMU::BSP_IMU.Handle();
//    AutoAim::autoAim.Handle();
}

void RemoteKeyMouseControlLoop() {
    //if (Remote::remote.DataUpdate()) {
    switch (Remote::remote.GetInputMode()) {
        case REMOTE_INPUT: {
            if (Remote::remote.GetWorkState() > 0) {
                Remote::remote.LeverControl();
            }
        }
            break;
        case KEY_MOUSE_INPUT: {
            if (Remote::remote.GetWorkState() > 0) {
                Remote::remote.KeyMouseControl();
            }
        }
            break;
        case STOP: {

        }
            break;
    }
}

void Count() {
    Shoot::shoot.Count();
    AutoAim::autoAim.Count();
}

void Car::Reset(void) {
    Motor::motorInit();
    Chassis::chassis.Reset();
//    Gimbal::gimbal.Reset(imu);
//    Shoot::shoot.Reset();
    CAN::can2.Reset(CAN_TYPE_2, 1, 1, 0);
    CAN::can1.Reset(CAN_TYPE_1, 1, 1, 0);
    Remote::remote.Reset();
    IMU::BSP_IMU.Reset();
    MX_IWDG_Init();
    Additional::additional.Reset();
//    AutoAim::autoAim.Reset();
//    InitJudgeUart();
//    Gate::gate.Reset();
    //Gate::gate.Reset("1000", "0500");
}


Car Car::car;
