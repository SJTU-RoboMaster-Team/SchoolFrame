/**
  ******************************************************************************
  * @FileName			    CapTask.h
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

#ifndef __CAPTASK_H__
#define __CAPTASK_H__

#include "includes.h"

#define CAP_ADC_HITS (50)
#define CAP_ADC_CHANNALS (4)

#define VAL_POWER_Voltage    (((float)ADCProcessedData[1]*3.3f*11.0f)/4095.0f)//PB0
#define VAL_CAP_Voltage           ((float)ADCProcessedData[2]*3.3f*11.0f/4095.0f)                            //PB1
#define VAL_OUT_Voltage      ((float)ADCProcessedData[3]*3.3f*11.0f/4095.0f)
#define VAL_POWER_CUR        ((VAL_CAP_Voltage>0.0f?GetAimedPower()/VAL_CAP_Voltage:10.0f)<=10.0f?(VAL_CAP_Voltage>0.0f?GetAimedPower()/VAL_CAP_Voltage:10.0f):10.0f)
#define DAC_OUT                   (uint32_t)(VAL_POWER_CUR*4095.0f/3.3f/5.0f*1.3f)


typedef enum {
    CAP_STATE_STOP = 1,
    CAP_STATE_RELEASE,
    CAP_STATE_PREPARE,
    CAP_STATE_EMERGENCY
} CapState_e;

class Cap {
public:
    static Cap cap;
    double RxAimedPower;

    float GetAimedPower(void);

    float GetCapVoltage(void);

    float GetOutVoltage(void);

    float GetPowerVoltage(void);

    CapState_e GetCapState(void);

    inline void SetRxCapVoltage(float value) { RxCapVoltage = value; }

    inline void SetRxPwrVoltage(float value) { RxCapVoltage = value; }

    inline void SetRxOutVoltage(float value) { RxCapVoltage = value; }

    inline void SetRxCapState(CapState_e state) { RxCapState = state; }

    void Reset(void);

    void Handle(void);

    GPIO_TypeDef *mosGPIO[4];
    uint16_t mosPin[4];
    uint16_t ADCVal[CAP_ADC_HITS][CAP_ADC_CHANNALS];
    uint32_t ADCTmp[CAP_ADC_CHANNALS];
    uint16_t ADCProcessedData[CAP_ADC_CHANNALS];
private:
    void FSMHandle(void);

    void ControlHandle(void);

    void SwitchStateTo(CapState_e state);

    void LEDShowCapVoltage(void);

    CapState_e capState, RxCapState;
    float RxCapVoltage;
    float RxPwrVoltage;
    float RxOutVoltage;

};

#endif
