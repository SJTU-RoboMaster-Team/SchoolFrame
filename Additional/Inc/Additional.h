//
// Created by 92304 on 2020/10/4.
//

#ifndef RM_FRAME2_ADDITIONAL_H
#define RM_FRAME2_ADDITIONAL_H

#include "includes.h"

class Additional {
public:
    static Additional additional;
    NormalMotor M2006;
    ChassisMotor Roll;
    ChassisMotor Brush;

    void Handle();

    void Reset();

    void setBrushVelocity(int16_t);

    void setRollVelocity(int16_t);

    void setTargetAngle(int16_t);
};

#endif //RM_FRAME2_ADDITIONAL_H
