/**
  ******************************************************************************
  * @FileName			    GimbalTask.cpp
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

#include "includes.h"

Gimbal::Gimbal(void) {

}

void Gimbal::Handle() {
    GMY.Handle();
    GMP.Handle();
}

void Gimbal::Reset(GMFeedbackType_e _feedbackType) {
    feedbackType = _feedbackType;
    GMY.Reset(CAN_TYPE_2, 0x205, 1,
              8000, 0, 20, 20000, 20000, 20000, 16384,
              0.4, 0, 0, 5000, 5000, 5000, 5000);
    GMP.Reset(CAN_TYPE_1, 0x206, 1,
              10000, 200, 200, 20000, 20000, 20000, 16384,
              0.4, 0, 0, 5000, 5000, 5000, 5000);
}

Gimbal Gimbal::gimbal;
