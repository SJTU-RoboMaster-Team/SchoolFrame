/**
  ******************************************************************************
  * @FileName			    GimbalTask.h
  * @Description            Control the gimbal motor
  * @author                 Steve Young
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#ifndef __GIMBAL_H__
#define __GIMBAL_H__

#include "includes.h"

#define YAW_DIR 1
#define PITCH_DIR 1

#define GM_PITCH_ZERO  1415//6250
#define GM_YAW_ZERO  1015//8160
#define GM_PITCH_GRAVITY_COMPENSATION 0.0

#define GIMBAL_SPEED_K                    0.006f

enum GM_e {
    GMPitch = 0,
    GMYaw
};

class Gimbal {
private:
    GMPitchMotor GMP;
    GMYawMotor GMY;

public:
    static Gimbal gimbal;
    GMFeedbackType_e feedbackType;

    Gimbal();

    void Handle();

    void Reset(GMFeedbackType_e _feedbackType);

    inline void SetPosition(double pitchPos, double yawPos, double pitchK, double yawK) {
        GMP.targetAngle += PITCH_DIR * pitchPos * pitchK;
        GMY.targetAngle += YAW_DIR * yawPos * yawK;
    }

    inline void AutoAimSetPosition(double pitchPos, double yawPos) {
        GMP.targetAngle = GMP.GetRealAngle() + pitchPos;
        GMY.targetAngle = GMY.GetRealAngle() + yawPos;
    }

    inline double GMRealAngle(GM_e GM) {
        if (GM == GMPitch) return GMP.GetRealAngle();
        else return GMY.GetRealAngle();
    }

    inline void SetPosition(double pitchPos) { GMP.targetAngle += PITCH_DIR * pitchPos * GIMBAL_SPEED_K; }

    inline ESCC6x0RxMsg_t GetRxMsg(GM_e GM) {
        if (GM == GMPitch) return GMP.GetRxMsgC6x0();
        else return GMY.GetRxMsgC6x0();
    }
};

#endif
