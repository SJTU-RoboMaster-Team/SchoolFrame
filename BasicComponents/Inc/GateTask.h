/**
  ******************************************************************************
  * @FileName			    GateTask.h
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

#ifndef __GATETASK_H__
#define __GATETASK_H__

#include "includes.h"

enum GateState_e {
    close = 0,
    open
};

class Gate {
private:
    GateState_e gateState;
    GateState_e lastGateState;
public:
    static Gate gate;

    void Reset();

    void SetGateState(GateState_e _gateState);

    void Handle();
};

#endif