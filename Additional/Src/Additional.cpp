//
// Created by 92304 on 2020/10/4.
//

#include "../Inc/Additional.h"

#include "includes.h"

void Additional::Handle() {
//    M2006.Handle();
    Brush.Handle();
    Roll.Handle();
}

void Additional::Reset() {
//    M2006.Reset(CAN_TYPE_1, 0x202, 36,
//                800.0, 0, 0.0, 1580.0, 1080.0, 1080.0, 1080.0,
//                15.0, 0, 0.0, 10000.0, 5000.0, 10000.0, 10000.0);
    Brush.Reset(CAN_TYPE_1, 0x205, 1, 20, 0, 0, 20000, 20000, 20000, 16384);
    Roll.Reset(CAN_TYPE_1, 0x206, 1, 20, 0, 0, 20000, 20000, 20000, 16384);
}

void Additional::setTargetAngle(int16_t addAngle) {
    M2006.targetAngle += addAngle;
}

void Additional::setBrushVelocity(int16_t velocity) {
    Brush.targetAngle = velocity;
}

void Additional::setRollVelocity(int16_t velocity) {
    Roll.targetAngle = velocity;
}


Additional Additional::additional;