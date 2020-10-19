/**
  ******************************************************************************
  * @FileName			    ShootTask.h
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

#ifndef __SHOOTTASK_H__
#define __SHOOTTASK_H__

#include "includes.h"

#define MAXHEAT01    180
#define MAXHEAT02    240
#define MAXHEAT03    300
#define COOLDOWN01    30.0f
#define COOLDOWN02    40.0f
#define COOLDOWN03    50.0f

#define MAXHEAT11    200
#define MAXHEAT12    300
#define MAXHEAT13    400
#define COOLDOWN11    20.0f
#define COOLDOWN12    40.0f
#define COOLDOWN13    60.0f

#define MAXSPEED1    15
#define MAXSPEED2    18
#define MAXSPEED3    22
#define MAXSPEED4    30

#define FRIC_SPEED_1    4400
#define FRIC_SPEED_2    4800
#define FRIC_SPEED_3    5400
#define FRIC_SPEED_4    7000


#define STIR_STEP_ANGLE 45
#define    LONG_CD            200
#define    SHORT_CD        50


class Shoot {
private:
    float fakeHeat0{};
    float fakeHeat1{};
    float coolDown0{};
    float coolDown1{};
    uint16_t maxHeat0{MAXHEAT03};
    uint16_t maxHeat1{MAXHEAT13};
    uint16_t maxSpeed0{MAXSPEED1};
    uint16_t maxSpeed1{MAXSPEED1};
    uint16_t fricSpeed{FRIC_SPEED_1};
    uint16_t heat0Count{};
    uint16_t heat1Count{};
    uint16_t stirCount{};
    uint8_t burst{};
    bool fric_ON{false};
    bool blockFlag{false};
    int16_t shootCD{};
    int16_t currentCD{SHORT_CD};

    NormalMotor STIR;
    ChassisMotor FRICL;
    ChassisMotor FRICR;

public:
    static Shoot shoot;

    Shoot();

    void Count();

    void SwitchBulletSpeed();

    void ShootOneBullet(uint8_t state);

    void BulletBlockHandler();

    void Fric(bool ON);

    void FakeHeatCalc();

    void Handle();

    void Reset();

    void SetBurst(uint8_t _burst);
};


extern Shoot shoot;

#endif
