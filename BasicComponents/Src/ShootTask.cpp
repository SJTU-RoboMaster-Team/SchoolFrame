/**
  ******************************************************************************
  * @FileName			    ShootTask.cpp
  * @Description            Shoot bullets and heat limitation
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

#define USE_HEAT_LIMIT

Shoot::Shoot() {

}

void Shoot::Reset() {
    STIR.Reset(CAN_TYPE_1, 0x205, 36,
               800.0, 0, 0.0, 1580.0, 1080.0, 1080.0, 1080.0,
               15.0, 0, 0.0, 10000.0, 5000.0, 10000.0, 10000.0);
    FRICL.Reset(CAN_TYPE_1, 0x201, 1,30.0, 0.0, 5.0, 15000, 15000, 15000, 10000);
    FRICR.Reset(CAN_TYPE_1, 0x202, 1,30.0, 0.0, 5.0, 15000, 15000, 15000, 10000);
}

void Shoot::Fric(bool ON) {
    fric_ON = ON;
    if (fric_ON) {
        FRICL.targetAngle = -fricSpeed;
        FRICR.targetAngle = fricSpeed;
    } else {
        FRICL.targetAngle = 0;
        FRICR.targetAngle = 0;
    }
}

void Shoot::Count() {
    if (shootCD > 0) shootCD--;
    if (stirCount > 0) stirCount--;
    if (heat0Count > 0) heat0Count--;
    if (heat1Count > 0) heat1Count--;
}

void Shoot::FakeHeatCalc() {
    //if (JUDGE_State == ONLINE) {
        maxHeat0 = RefereeData.GameRobotState.shooter_heat0_cooling_limit;
        coolDown0 = RefereeData.GameRobotState.shooter_heat0_cooling_rate;
        maxHeat1 = RefereeData.GameRobotState.shooter_heat1_cooling_limit;
        coolDown1 = RefereeData.GameRobotState.shooter_heat1_cooling_rate;
        if (heat0Count == 0) fakeHeat0 = RefereeData.PowerHeat.shooter_heat0;
        if (heat1Count == 0) fakeHeat1 = RefereeData.PowerHeat.shooter_heat1;
    //}
    if (fakeHeat0 >= coolDown0 / 1000) fakeHeat0 -= coolDown0 / 1000;
    else fakeHeat0 = 0;
    if (fakeHeat1 >= coolDown1 / 1000) fakeHeat1 -= coolDown1 / 1000;
    else fakeHeat1 = 0;
}

void Shoot::SwitchBulletSpeed() {
    maxSpeed0 = RefereeData.GameRobotState.shooter_heat0_speed_limit;
    maxSpeed1 = RefereeData.GameRobotState.shooter_heat1_speed_limit;
    switch (maxSpeed0) {
        case MAXSPEED1:
            fricSpeed = FRIC_SPEED_1;
            break;
        case MAXSPEED2:
            fricSpeed = FRIC_SPEED_2;
            break;
        case MAXSPEED3:
            fricSpeed = FRIC_SPEED_3;
            break;
        case MAXSPEED4:
            fricSpeed = FRIC_SPEED_4;
            break;
    }
    //LastFricSpeed = RefereeData.GameRobotState.shooter_heat0_speed_limit;
}

void Shoot::ShootOneBullet(uint8_t state) {
#ifndef USE_HEAT_LIMIT
    STIR.targetAngle -= STIR_STEP_ANGLE;
#else
    if (state == 0) {
        if ((maxHeat0 - fakeHeat0 >= 20 || burst) && fric_ON && !blockFlag && shootCD == 0) {
            STIR.targetAngle -= STIR_STEP_ANGLE;
            fakeHeat0 += 10;
            heat0Count = 200;
            shootCD = currentCD;
        }
    }
    if (state == 1) {
        if ((maxHeat1 - fakeHeat1 >= 20 || burst) && fric_ON && !blockFlag && shootCD == 0) {
            STIR.targetAngle -= STIR_STEP_ANGLE;
            fakeHeat1 += 100;
            heat1Count = 200;
            shootCD = currentCD;
        }
    }
#endif
}

void Shoot::BulletBlockHandler() {
    OnePush(STIR.GetRxMsgC6x0().moment < -6000,
            {
                STIR.targetAngle += 1.5 * STIR_STEP_ANGLE;
                stirCount = 100;
                blockFlag = 1;
            });
    if (blockFlag == 1) {
        OnePush(stirCount == 0,
                {
                    if (STIR.GetRxMsgC6x0().moment > -500) {
                        STIR.targetAngle -= 0.5 * STIR_STEP_ANGLE;
                        blockFlag = 0;
                    } else {
                        STIR.targetAngle += STIR_STEP_ANGLE;
                        stirCount = 100;
                    }
                });
    }
}

void Shoot::SetBurst(uint8_t _burst) {
    burst = _burst;
}

void Shoot::Handle() {
    SwitchBulletSpeed();
    BulletBlockHandler();
    FakeHeatCalc();
    FRICL.Handle();
    FRICR.Handle();
    STIR.Handle();
}

Shoot Shoot::shoot;
