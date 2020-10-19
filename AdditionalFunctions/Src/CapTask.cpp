/**
  ******************************************************************************
  * @FileName			    CapTask.cpp
  * @Description            Super Capacity
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

float Cap::GetAimedPower(void) {
#ifndef BOARD_SLAVE
    if(JUDGE_State == OFFLINE){
        return 40;
    }else{
        return RefereeData.GameRobotState.max_chassis_power + (10*(RefereeData.PowerHeat.chassis_power_buffer - 10))>40?40:(10*(RefereeData.PowerHeat.chassis_power_buffer - 10));
    }
#else
    return RxAimedPower;
#endif
}

float Cap::GetCapVoltage(void) {
#ifdef BOARD_MAIN
    return RxCapVoltage;
#else
    return VAL_CAP_Voltage;
#endif
}

float Cap::GetOutVoltage(void) {
#ifdef BOARD_MAIN
    return RxOutVoltage;
#else
    return VAL_OUT_Voltage;
#endif
}

float Cap::GetPowerVoltage(void) {
#ifdef BOARD_MAIN
    return RxPwrVoltage;
#else
    if(VAL_POWER_Voltage>0)
        return VAL_POWER_Voltage;
    else
        return 0.1;
#endif
}

CapState_e Cap::GetCapState() {
#ifdef BOARD_MAIN
    return RxCapState;
#else
    return capState;
#endif
}


void Cap::Reset(void) {
    RxCapVoltage = 0.1;
    RxOutVoltage = 0.1;
    RxPwrVoltage = 0.1;
    RxAimedPower = 0;
    capState = CAP_STATE_STOP;
    mosGPIO[0] = GPIOE;
    mosGPIO[1] = GPIOE;
    mosGPIO[2] = GPIOC;
    mosGPIO[3] = GPIOC;
    mosPin[0] = GPIO_PIN_12;
    mosPin[1] = GPIO_PIN_6;
    mosPin[2] = GPIO_PIN_2;
    mosPin[3] = GPIO_PIN_3;
    for (int i = 0; i < 4; ++i) HAL_GPIO_WritePin(mosGPIO[i], mosPin[i], GPIO_PIN_RESET);
    HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, 0);
    HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
    memset(ADCVal, 0, sizeof(ADCVal));
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *) ADCVal, CAP_ADC_CHANNALS * CAP_ADC_HITS);
}

void Cap::Handle(void) {
    uint8_t cnt1, cnt2;
    memset(ADCTmp, 0, sizeof(ADCTmp));
    for (cnt1 = 0; cnt1 < CAP_ADC_CHANNALS; cnt1++) {
        for (cnt2 = 0; cnt2 < CAP_ADC_HITS; cnt2++) {
            ADCTmp[cnt1] += ADCVal[cnt2][cnt1];
        }
        ADCProcessedData[cnt1] = ADCTmp[cnt1] / CAP_ADC_HITS;
    }
    FSMHandle();
    ControlHandle();
    LEDShowCapVoltage();
}

void Cap::FSMHandle(void) {
    if (Remote::remote.GetWorkState() == STOP_STATE) SwitchStateTo(CAP_STATE_STOP);
    else {
        switch (capState) {
            case CAP_STATE_STOP:
                if (VAL_POWER_Voltage > 10) SwitchStateTo(CAP_STATE_PREPARE);
                break;
            case CAP_STATE_PREPARE:
                if (VAL_POWER_Voltage < 9) {
                    SwitchStateTo(CAP_STATE_STOP);
                } else if (VAL_CAP_Voltage > 15) {
                    SwitchStateTo(CAP_STATE_RELEASE);
                }
                break;
            case CAP_STATE_RELEASE:
#ifdef USE_EMERGENCY_MODE
                static int cap_emergency_cnt=0;
#endif
                if (VAL_POWER_Voltage < 9) {
                    SwitchStateTo(CAP_STATE_STOP);
                } else {
#ifdef USE_EMERGENCY_MODE
                    if(VAL_CAP_Voltage<10){
            if(cap_emergency_cnt<1100){
                cap_emergency_cnt++;
            }
            if(cap_emergency_cnt>1000){
                Cap_State_Switch(CAP_STATE_EMERGENCY);
            }
        }else{
            cap_emergency_cnt=0;
        }
#endif
                }
                break;
            case CAP_STATE_EMERGENCY:
                //
                break;
        }
    }
}

void Cap::ControlHandle(void) {
    static uint8_t boost_mode_flag = 0;
    switch (capState) {
        case CAP_STATE_STOP:
            HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, 0);
            break;
        case CAP_STATE_RELEASE:
            HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
            break;
        case CAP_STATE_EMERGENCY:
            HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, 0);
            break;

        case CAP_STATE_PREPARE:
            HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, DAC_OUT);
            break;
    }
    if (GetOutVoltage() > 28) boost_mode_flag = 100;
    if (GetOutVoltage() < 27) boost_mode_flag = 0;
    if (boost_mode_flag == 0)HAL_GPIO_WritePin(mosGPIO[3], mosPin[3], GPIO_PIN_RESET);
    else HAL_GPIO_WritePin(mosGPIO[3], mosPin[3], GPIO_PIN_SET);
}

void Cap::SwitchStateTo(CapState_e state) {
    switch (state) {
        case CAP_STATE_STOP:
            capState = CAP_STATE_STOP;
            HAL_GPIO_WritePin(mosGPIO[0], mosPin[0], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(mosGPIO[1], mosPin[1], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(mosGPIO[2], mosPin[2], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(mosGPIO[3], mosPin[3], GPIO_PIN_RESET);
            break;
        case CAP_STATE_RELEASE:
            capState = CAP_STATE_RELEASE;
            HAL_GPIO_WritePin(mosGPIO[0], mosPin[0], GPIO_PIN_SET);
            HAL_GPIO_WritePin(mosGPIO[1], mosPin[1], GPIO_PIN_SET);
            HAL_GPIO_WritePin(mosGPIO[2], mosPin[2], GPIO_PIN_RESET);
            //HAL_GPIO_WritePin(mosGPIO[3],mosPin[3],GPIO_PIN_RESET);
            break;
        case CAP_STATE_EMERGENCY:
            capState = CAP_STATE_EMERGENCY;
            HAL_GPIO_WritePin(mosGPIO[0], mosPin[0], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(mosGPIO[1], mosPin[1], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(mosGPIO[2], mosPin[2], GPIO_PIN_SET);
            //HAL_GPIO_WritePin(mosGPIO[3], mosPin[3], GPIO_PIN_RESET);
            break;

        case CAP_STATE_PREPARE:
            capState = CAP_STATE_PREPARE;
            HAL_GPIO_WritePin(mosGPIO[0], mosPin[0], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(mosGPIO[1], mosPin[1], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(mosGPIO[2], mosPin[2], GPIO_PIN_RESET);
            //HAL_GPIO_WritePin(mosGPIO[3], mosPin[3], GPIO_PIN_RESET);
            break;
    }
}

void Cap::LEDShowCapVoltage(void) {
    if (GetCapState() == CAP_STATE_EMERGENCY) {
        HAL_GPIO_WritePin(GPIOG, 0x1fe, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOG, 0x1e0, GPIO_PIN_RESET);
        return;
    }
    if (GetCapVoltage() < 11)
        HAL_GPIO_WritePin(GPIOG, 0x1fe, GPIO_PIN_SET);
    else {
        HAL_GPIO_WritePin(GPIOG, 0x1fe, GPIO_PIN_SET);
        int unlight = 7 - (int) (((GetCapVoltage() * GetCapVoltage() - 121) / (23 * 23 - 121)) * 6);
        if (unlight < 0) unlight = 0;
        HAL_GPIO_WritePin(GPIOG, 0x1fe >> unlight, GPIO_PIN_RESET);
    }
}

Cap Cap::cap;
