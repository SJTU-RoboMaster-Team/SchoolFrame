/**
  ******************************************************************************
  * @FileName			    CANTask.cpp
  * @Description            CAN communication
  * @author                 Chang Liu
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#include "includes.h"

void CAN::RxHandle(void) {
    //uint8_t sort = (hcan == &hcan1) ? 0 : 1; //use hcan to determine whether CAN1 or CAN2
    static uint8_t sort;
    if (hcan == &hcan1) {
        sort = 0;
    } else {
        sort = 1;
    }
    if (everRx == 0)
        everRx = 1;
    Motor::motors[sort][RxMsg.StdId - 0x201]->SetRxMsgC6x0(CanRxGetU16(RxMsg, 0), CanRxGetU16(RxMsg, 1),
                                                           CanRxGetU16(RxMsg, 2));
    Motor::motors[sort][RxMsg.StdId - 0x201]->everRx = 1;
    if (RxMsg.StdId == 0x300) {
#ifdef BOARD_SLAVE
        //<double board data decode>
        switch (RxMsg.Data[0]) {
            case 0xff:
                Remote::remote.SetRxWorkState(STOP_STATE);
                break;
            case 0x00:
                Remote::remote.SetRxWorkState(PREPARE_STATE);
                break;
            case 0x01:
                Remote::remote.SetRxWorkState(NORMAL_STATE);
                break;
            case 0x02:
                Remote::remote.SetRxWorkState(ADDITIONAL_STATE_ONE);
                break;
            case 0x03:
                Remote::remote.SetRxWorkState(ADDITIONAL_STATE_TWO);
                break;
            default:
                Remote::remote.SetRxWorkState(STOP_STATE);
                break;
        }
        Cap::cap.RxAimedPower = RxMsg.Data[1];
        Remote::remote.SetWorkState(Remote::remote.GetRxWorkState());
        //</double board data decode>
#endif
    } else if (RxMsg.StdId == 0x301) {
#ifdef BOARD_MAIN
        //<double board data decode>
        Cap::cap.SetRxPwrVoltage(RxMsg.Data[0] / 5.0);
        Cap::cap.SetRxCapVoltage(RxMsg.Data[1] / 5.0);
        Cap::cap.SetRxOutVoltage(RxMsg.Data[2] / 5.0);
        switch (RxMsg.Data[3]) {
            case 0xff:
                Cap::cap.SetRxCapState(CAP_STATE_STOP);
                break;
            case 0x01:
                Cap::cap.SetRxCapState(CAP_STATE_RELEASE);
                break;
            case 0x02:
                Cap::cap.SetRxCapState(CAP_STATE_PREPARE);
                break;
            case 0x03:
                Cap::cap.SetRxCapState(CAP_STATE_EMERGENCY);
                break;
        }
        //</double board data decode>
#endif
    }
}

void CAN::TxHandleMotor(uint8_t part) {
    uint8_t sort = (hcan == &hcan2); //determine whether it's CAN1 or CAN2
    hcan->pTxMsg = &TxMsg;
    //<frame header>
    if (part == 0)
        TxMsg.StdId = 0x200;
    else
        TxMsg.StdId = 0x1ff;
    TxMsg.ExtId = 0;
    TxMsg.IDE = CAN_ID_STD;
    TxMsg.RTR = CAN_RTR_DATA;
    TxMsg.DLC = 0x08;
    //</frame header>

    //<encode data to be sent to motor>
    for (int i = 0; i < 4; ++i) {
        if (Motor::motors[sort][i + part * 4] == NULL) {
            TxMsg.Data[i * 2] = 0;
            TxMsg.Data[i * 2 + 1] = 0;
        } else {
            TxMsg.Data[i * 2] = (uint8_t) (Motor::motors[sort][i + part * 4]->GetIntensity() >> 8);
            TxMsg.Data[i * 2 + 1] = (uint8_t) (Motor::motors[sort][i + part * 4]->GetIntensity());
        }
    }
    //</encode data to be sent to motor>

    //<transmit>
    if (TxDone == 1) {
        HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
        HAL_NVIC_DisableIRQ(USART1_IRQn);
        HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);
        HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
        HAL_NVIC_DisableIRQ(TIM7_IRQn);

        if (HAL_CAN_Transmit_IT(hcan) != HAL_OK) {
            Error_Handler();
        }

        TxDone = 0;

        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
    //</transmit>
}

void CAN::TxHandleDoubleBoard(void) {
    hcan->pTxMsg = &TxMsg;
#ifdef BOARD_MAIN
    //<encode double board data>
    TxMsg.StdId = 0x300;
    TxMsg.ExtId = 0;
    TxMsg.IDE = CAN_ID_STD;
    TxMsg.RTR = CAN_RTR_DATA;
    TxMsg.DLC = 0x08;
    switch (Remote::remote.GetWorkState()) {
        case STOP_STATE:
            TxMsg.Data[0] = 0xff;
            break;
        case PREPARE_STATE:
            TxMsg.Data[0] = 0x00;
            break;
        case NORMAL_STATE:
            TxMsg.Data[0] = 0x01;
            break;
        case ADDITIONAL_STATE_ONE:
            TxMsg.Data[0] = 0x02;
            break;
        case ADDITIONAL_STATE_TWO:
            TxMsg.Data[0] = 0x03;
            break;
    }
    TxMsg.Data[1] = Cap::cap.GetAimedPower();
    TxMsg.Data[2] = 0;
    TxMsg.Data[3] = 0;
    TxMsg.Data[4] = 0;
    TxMsg.Data[5] = 0;
    TxMsg.Data[6] = 0;
    TxMsg.Data[7] = 0;
    //</encode double board data>
#endif
#ifdef BOARD_SLAVE
    //<encode double board data>
    TxMsg.StdId = 0x301;
    TxMsg.ExtId = 0;
    TxMsg.IDE = CAN_ID_STD;
    TxMsg.RTR = CAN_RTR_DATA;
    TxMsg.DLC = 0x08;
    TxMsg.Data[0] = Cap::cap.GetPowerVoltage() * 5.0;
    TxMsg.Data[1] = Cap::cap.GetCapVoltage() * 5.0;
    TxMsg.Data[2] = Cap::cap.GetOutVoltage() * 5.0;
    TxMsg.Data[3] = 0;
    TxMsg.Data[4] = 0;
    TxMsg.Data[5] = 0;
    TxMsg.Data[6] = 0;
    TxMsg.Data[7] = 0;
    //</encode double board data>
#endif

    //</transmit>
    if (TxDone == 1) {
        HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
        HAL_NVIC_DisableIRQ(USART1_IRQn);
        HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);
        HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
        HAL_NVIC_DisableIRQ(TIM7_IRQn);

        if (HAL_CAN_Transmit_IT(hcan) != HAL_OK) {
            Error_Handler();
        }

        TxDone = 0;

        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
    //</transmit>
}

void CAN::Reset(CanType_e type, uint8_t motorPart1, uint8_t motorPart2, uint8_t doubleBoard) {
    currentSendData = 0;
    sendData[0] = motorPart1;
    sendData[1] = motorPart2;
    sendData[2] = doubleBoard;
    TxDone = 1;
    if (type == CAN_TYPE_1)
        hcan = &hcan1;
    else
        hcan = &hcan2;
    hcan->pRxMsg = &RxMsg;
    //<CAN filter>
    CAN_FilterConfTypeDef sFilterConfig;
    if (type == CAN_TYPE_1)
        sFilterConfig.FilterNumber = 0;
    else
        sFilterConfig.FilterNumber = 14;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = 0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.BankNumber = 14;
    HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
    //</CAN filter>
    if (HAL_CAN_Receive_IT(hcan, CAN_FIFO0) != HAL_OK) //first receive
    {
        Error_Handler();
    }
    everRx = 0;
}

void CAN::TxHandle(void) {
    uint8_t cnt = 0;
    //<switch object to send CAN data to>
    do {
        currentSendData = (currentSendData + 1) % 3;
        cnt++;
    } while (sendData[currentSendData] == 0 && cnt <= 3);
    //</switch object to send CAN data to>

    switch (currentSendData) {
        case 0:
            if (sendData[currentSendData])
                TxHandleMotor(0);
            break;
        case 1:
            if (sendData[currentSendData])
                TxHandleMotor(1);
            break;
        case 2:
            if (sendData[currentSendData])
                TxHandleDoubleBoard();
            break;
    }
}

CAN CAN::can1;
CAN CAN::can2;
