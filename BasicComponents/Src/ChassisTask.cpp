/**
  ******************************************************************************
  * @FileName			    ChassisTask.cpp
  * @Description            Control the chassis motors and limit the power of them
  *                         Follow the gimbal or twist or spin
  * @author                 Steve Young
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#include <ChassisTask.h>

#include "includes.h"

void Chassis::Handle(void) {
    ChassisDataDecoding();
    PowerLimitation();

    CMFL.Handle();
    CMFR.Handle();
    CMBL.Handle();
    CMBR.Handle();
}

void Chassis::Reset(void) {
    lock = 0;
    followCenter = GM_YAW_ZERO;
    SetVelocity(0, 0, 0);
    CMFL.Reset(CAN_TYPE_1, 0x202, 1, 5, 0, 0, 20000, 20000, 20000, 16384);
    CMFR.Reset(CAN_TYPE_1, 0x201, 1, 5, 0, 0, 20000, 20000, 20000, 16384);
    CMBL.Reset(CAN_TYPE_1, 0x203, 1, 5, 0, 0, 20000, 20000, 20000, 16384);
    CMBR.Reset(CAN_TYPE_1, 0x204, 1, 5, 0, 0, 20000, 20000, 20000, 16384);
    rotate.Reset(1, 0.0f, 0.0f, 3000.0, 3000.0, 3000.0, 1500.0);
}

void Chassis::PowerLimitation() {
    powerLimPara.kIntensityToMoment = 200000.0;
    powerLimPara.PowerMax = 0;  //powerLimPara.PowerMax=(double)Cap_Get_Aim_Power()*0.7;
    powerLimPara.rotateSpeedRate = 0.0;
    powerLimPara.CMFLRotateSpeed = (double) CMFL.GetRxMsgC6x0().rotateSpeed;
    powerLimPara.CMFRRotateSpeed = (double) CMFR.GetRxMsgC6x0().rotateSpeed;
    powerLimPara.CMBLRotateSpeed = (double) CMBL.GetRxMsgC6x0().rotateSpeed;
    powerLimPara.CMBRRotateSpeed = (double) CMBR.GetRxMsgC6x0().rotateSpeed;

    double PowerSum = CMFL.GetSpeedPID().PIDInfo.kp *
                      ((CMFL.targetAngle - powerLimPara.CMFLRotateSpeed) * powerLimPara.CMFLRotateSpeed + \

                       (CMFR.targetAngle - powerLimPara.CMFRRotateSpeed) * powerLimPara.CMFRRotateSpeed + \

                       (CMBL.targetAngle - powerLimPara.CMBLRotateSpeed) * powerLimPara.CMBLRotateSpeed + \

                       (CMBR.targetAngle - powerLimPara.CMBRRotateSpeed) * powerLimPara.CMBRRotateSpeed) /
                      powerLimPara.kIntensityToMoment;

#ifdef POWER_LIMITATION_DEBUG

    if (RefereeData.GameRobotState.max_chassis_power != 0)
        powerLimPara.PowerMax =
                RefereeData.GameRobotState.max_chassis_power + 10 * (RefereeData.PowerHeat.chassis_power_buffer - 10);
    else
        powerLimPara.PowerMax = 5000;
    //PowerMax = RefereeData.GameRobotState.max_chassis_power - 10;
#else
#ifdef USE_CAP3
    if(Cap_Get_Cap_State()==CAP_STATE_RELEASE && Cap_Get_Cap_Voltage()>12.0){
                PowerMax = 10000.0;
            }
#endif
#endif
    if (PowerSum > powerLimPara.PowerMax) {
        powerLimPara.rotateSpeedRate =
                1.0 -
                (PowerSum - powerLimPara.PowerMax) /
                (CMFL.GetSpeedPID().PIDInfo.kp * (CMFL.targetAngle * powerLimPara.CMFLRotateSpeed + \

                                                  CMFR.targetAngle * powerLimPara.CMFRRotateSpeed + \

                                                  CMBL.targetAngle * powerLimPara.CMBLRotateSpeed + \

                                                  CMBR.targetAngle *
                                                  powerLimPara.CMBRRotateSpeed)) * powerLimPara.kIntensityToMoment;
        if (powerLimPara.rotateSpeedRate > 1.0)powerLimPara.rotateSpeedRate = 1.0;
        if (powerLimPara.rotateSpeedRate < 0.0)powerLimPara.rotateSpeedRate = 0.0;
        CMFL.targetAngle *= powerLimPara.rotateSpeedRate;
        CMFR.targetAngle *= powerLimPara.rotateSpeedRate;
        CMBL.targetAngle *= powerLimPara.rotateSpeedRate;
        CMBR.targetAngle *= powerLimPara.rotateSpeedRate;
    }
}

void Chassis::TwistSpin() {
    angleToCenter = fabs(
            YAW_DIR * (followCenter - Gimbal::gimbal.GetRxMsg(GMYaw).angle) * 360 / 8192.0f - (float) twistGapAngle);
    if (angleToCenter > 360) angleToCenter -= 360;
    switch (twistState) {
        case 1: {
            rotateVelocity = 50;
            break;
        }
        case 2: {
            rotateVelocity = -50;
            break;
        }
        case 3: {
            switch (twistGapAngle) {
                case 0: {
                    twistGapAngle = -CHASSIS_TWIST_ANGLE_LIMIT;
                    break;
                }
                case CHASSIS_TWIST_ANGLE_LIMIT: {
                    if (angleToCenter < 10)
                        twistGapAngle = -CHASSIS_TWIST_ANGLE_LIMIT;
                    break;
                }
                case -CHASSIS_TWIST_ANGLE_LIMIT: {
                    if (angleToCenter < 10)
                        twistGapAngle = CHASSIS_TWIST_ANGLE_LIMIT;
                    break;
                }
            }
            rotateVelocity =
                    (float) (Gimbal::gimbal.GetRxMsg(GMYaw).angle - followCenter) * 360 / 8192.0f -
                    (float) twistGapAngle;
            break;
        }
        default: {
            twistGapAngle = 0;
            rotateVelocity =
                    (float) (Gimbal::gimbal.GetRxMsg(GMYaw).angle - followCenter) * 360 / 8192.0f -
                    (float) twistGapAngle;
            break;
        }
    }
}

void Chassis::ControlRotate() {
    if (abs(rotateVelocity) < 15) {
        rotate.PIDInfo.componentKi = 0;
    }
    rotate.PIDInfo.ref = rotateVelocity;
    rotate.PIDInfo.fdb = 0;
    rotate.Calc();
    rotateVelocity = YAW_DIR * rotate.PIDInfo.output;
}

void Chassis::Lock() { lock = 1; }

void Chassis::Unlock() { lock = 0; }

void Chassis::ChassisDataDecoding() {
    if (!lock) {
        ControlRotate();

        float cosPlusSin, cosMinusSin, GMYEncoderAngle;

        GMYEncoderAngle = (float) (Gimbal::gimbal.GetRxMsg(GMYaw).angle - GM_YAW_ZERO) * 6.28f / 8192.0f;

        cosPlusSin = cos(GMYEncoderAngle) + sin(GMYEncoderAngle);
        cosMinusSin = cos(GMYEncoderAngle) - sin(GMYEncoderAngle);


        CMFL.targetAngle = (forwardBackVelocity + leftRightVelocity + rotateVelocity) * 12;
        CMFR.targetAngle = (-forwardBackVelocity + leftRightVelocity + rotateVelocity) * 12;
        CMBL.targetAngle = (forwardBackVelocity - leftRightVelocity + rotateVelocity) * 12;
        CMBR.targetAngle = (-forwardBackVelocity - leftRightVelocity + rotateVelocity) * 12;

    } else {
        CMFL.targetAngle = 0;
        CMFR.targetAngle = 0;
        CMBL.targetAngle = 0;
        CMBR.targetAngle = 0;
    }
}

void Chassis::SetVelocity(int16_t _forwardBackVelocity, int16_t _leftRightVelocity, int16_t _rotateVelocity) {
    forwardBackVelocity = (float) _forwardBackVelocity * CHASSIS_SPEED_K;
    leftRightVelocity = (float) _leftRightVelocity * CHASSIS_SPEED_K / 3 * 2;
    rotateVelocity = (float) _rotateVelocity * ROTATE_SPEED_K;
}

void Chassis::AddVelocity(int16_t accFB, int16_t accLR) {
    forwardBackVelocity += (float) accFB;
    leftRightVelocity += (float) accLR;
    MinMax(forwardBackVelocity, -1000.0f, 1000.0f);
    MinMax(leftRightVelocity, -1000.0f, 1000.0f);
}

Chassis Chassis::chassis;
