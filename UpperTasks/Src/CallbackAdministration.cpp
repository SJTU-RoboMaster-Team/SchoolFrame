/**
  ******************************************************************************
  * @FileName			    CallbackAdministration.cpp
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

#include "includes.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == htim6.Instance) {
        HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
        MainControlLoop();
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
    if (htim->Instance == htim7.Instance) {
        Count();
        Remote::remote.Handle();
        if (lockCounter > 0)--lockCounter;
        else if (lockCounter == 0)Chassis::chassis.Unlock();
        //RemoteKeyMouseControlLoop();      V 
    }
}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *hcan) {
    if (hcan == CAN::can1.GetHcan()) {
        CAN::can1.RxHandle();
    } else {
        CAN::can2.RxHandle();
    }
    HAL_CAN_Receive_IT(hcan, CAN_FIFO0);
}

void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef *hcan) {
    if (hcan == &hcan1)
        CAN::can1.SetTxDone(1);
    if (hcan == &hcan2)
        CAN::can2.SetTxDone(1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) {
    uint8_t result;
    if (UartHandle == &RC_UART) {
        Remote::remote.UartRxCpltCallback();
    }
    if (UartHandle == &RECV_UART) {
        RecvRxCpltCallback();
    }
    if (UartHandle == &VISUAL_UART) {
        dealWithVisualResult();
    }
//    if (UartHandle == &AUTOAIM_UART) {
//        AutoAim::autoAim.UartRxCpltCallback();
//    }
//    if (UartHandle == &JUDGE_UART) {
//		JudgeUartRxCpltCallback();
//    }
}
