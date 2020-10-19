/**
  ******************************************************************************
  * @FileName			    CallbackAdministration.h
  * @Description            CAN & Uart & TIM callback
  * @author                 Chang Liu & Steve Young
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#ifndef __ITTASK_H__
#define __ITTASK_H__

#include "includes.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan);

#endif
