/**
  ******************************************************************************
  * @FileName			    GateTask.cpp
  * @Description            Open or close the bullets hatch(gate)
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

void Gate::Reset() {
    HAL_UART_Transmit_IT(&SERVO_UART, (uint8_t *) "#000P1000T0010!", sizeof("#000P1000T0010!"));
}

void Gate::SetGateState(GateState_e _gateState) {
    gateState = _gateState;
    if (gateState == GATE_OPEN && lastGateState == GATE_CLOSE)
        HAL_UART_Transmit_IT(&SERVO_UART, (uint8_t *) "#000P0500T0010!", sizeof("#000P0500T0010!"));
    else if (gateState == GATE_CLOSE && lastGateState == GATE_OPEN)
        HAL_UART_Transmit_IT(&SERVO_UART, (uint8_t *) "#000P1000T0010!", sizeof("#000P1000T0010!"));
    lastGateState = gateState;
}

void Gate::Handle() {
    if(Remote::remote.Channel().lcol < -658) Gate::gate.SetGateState(open);
    else Gate::gate.SetGateState(close);
}

Gate Gate::gate;