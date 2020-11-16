/**
  ******************************************************************************
  * @FileName			    LeverControlTask.cpp
  * @Description            Control the robot by the lever on the remote
  * @author                 Steve Young
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#include "includes.h"

#define USE_CHASSIS_FOLLOW

void Remote::LeverControl() {
    static WorkState_e lastWorkState = NORMAL_STATE;
    if (workState <= 0) return;


    Chassis::chassis.SetVelocity(channel.rcol, channel.rrow, channel.lrow * 5);

    if (workState == NORMAL_STATE) {
        //please write your code here;
        Additional::additional.setBrushVelocity(0);
        Additional::additional.setRollVelocity(0);
        if (channel.lcol >= 500) {
            Chassis::chassis.Lock();
            lockCounter = 5000;
        }

    } else if (workState == ADDITIONAL_STATE_ONE) {
        Additional::additional.setBrushVelocity(5000);
        Additional::additional.setRollVelocity(-5000);
        Chassis::chassis.Unlock();
        //please write your code here;
    } else if (workState == ADDITIONAL_STATE_TWO) {
        //please write your code here;
    }
    lastWorkState = workState;
}
