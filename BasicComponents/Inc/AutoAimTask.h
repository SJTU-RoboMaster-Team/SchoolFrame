/**
  ******************************************************************************
  * @FileName			    AutoAimTask.h
  * @Description            Commmunicate with MiniPC and control the gimbal
  *                         and shoot automatically
  * @author                 Steve Young
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#ifndef __AUTOAIMTASK_H__
#define __AUTOAIMTASK_H__

#include "includes.h"

#define AIM_INTERPOLATION_CNT   (20)

typedef struct GMAngle_t {
    double yaw;
    double pitch;
} GMAngle_t;

typedef struct Coordinate_t {
    float x;
    float y;
    float z;
} Coordinate_t;

typedef struct MCURecvData_t {
    float yaw;
    float pitch;
    float yaw_v;
    float pitch_v;
    uint8_t flag;
} __packed MCURecvData_t;

typedef struct MCUSendData_t {
    uint8_t start;
    float bulletSpeed;         // ??????
    uint8_t enemyColor: 1;     // ???????
    uint8_t program_mode: 1;    // ???????(??or????)
    uint8_t energy_mode: 6;     // ?????????
    uint8_t crc;
    uint8_t end;
} __packed MCUSendData_t;

enum AutoAimMode_e {
    manual = 0,
    predict,
    antiGro,
    energy
};

class AutoAimRecv {
private:
    static constexpr uint8_t length{1 + 4 + 4 + 4 + 4 + 1 + 1 + 1};
    uint8_t recvTemp;
    uint8_t recvStart;
    uint8_t bufferCnt;
    uint8_t LRC_Check;
    uint8_t buffer[length];
    uint8_t receivingCnt;
    uint16_t receiveCnt;

protected:
    MCURecvData_t data;

    void RecvInit();

public:
    bool finish;

    void UartRxCpltCallback();

    void Count() { if (receivingCnt > 0) receivingCnt--; }

    MCURecvData_t Data() { return data; }
};

class AutoAim : public AutoAimRecv {
private:
    bool autoShoot;
    bool gimbalFreeze;
    bool chassisFreeze;
    GMAngle_t output{};
    GMAngle_t adjust{};
    MCUSendData_t sendData{};
    uint8_t interpolationCnt;
    uint8_t interpolation;
    double worldYaw;

    //uint8_t JudgeBuffModeAccTime();
    void DataProcess();

    void Control();

    void UartTxInfo();

public:
    static AutoAim autoAim;
    AutoAimMode_e mode;
    bool V_Predict;
    float K_V_Predict;

    AutoAim();

    void Reset();

    void AutoShoot() const;

    void Handle();
};

extern AutoAim autoAim;

#endif
