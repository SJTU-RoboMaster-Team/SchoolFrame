/**
  ******************************************************************************
  * @FileName			    KeyMouseControlTask.cpp
  * @Description            Control the robot by the key and mouse on PC
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
#include "AutoAimTask.h"

int16_t speed = 0;

void Remote::KeyMouseControl() {
    static WorkState_e lastWorkState = NORMAL_STATE;
    if (workState <= 0) return;
    if (workState == ADDITIONAL_STATE_TWO) {
    }
    lastWorkState = workState;
}

void Remote::KeyboardModeFSM() {
    if ((key.v & 0x30) == 0x30) {
        KeyboardMode = SHIFT_CTRL;
    } else if (key.v & KEY_SHIFT) {
        KeyboardMode = SHIFT;
    } else if (key.v & KEY_CTRL) {
        KeyboardMode = CTRL;
    } else {
        KeyboardMode = NO_CHANGE;
    }

    if (KeyboardMode != LastKeyboardMode && KeyboardMode == SHIFT_CTRL) {}
    if (KeyboardMode == SHIFT) {} else {}
    if (KeyboardMode == CTRL) {
        KM_FORWORD_BACK_SPEED = LOW_FORWARD_BACK_SPEED;
        KM_LEFT_RIGHT_SPEED = LOW_LEFT_RIGHT_SPEED;
    }
    LastKeyboardMode = KeyboardMode;
}

void Remote::MouseModeFSM() {
    static uint8_t counterl = 0;
    static uint8_t counterr = 0;
    switch (MouseLMode) {
        case SHORT_CLICK: {
            counterl++;
            if (mouse.press_l == 0) {
                MouseLMode = NO_CLICK;
                counterl = 0;
            } else if (counterl >= 50) {
                MouseLMode = LONG_CLICK;
                counterl = 0;
            } else {
                MouseLMode = SHORT_CLICK;
            }
        }
            break;
        case LONG_CLICK: {
            if (mouse.press_l == 0) {
                MouseLMode = NO_CLICK;
            } else {
                MouseLMode = LONG_CLICK;
            }
        }
            break;
        case NO_CLICK: {
            if (mouse.press_l) {
                Shoot::shoot.ShootOneBullet(0);
                MouseLMode = SHORT_CLICK;
            }
        }
            break;
    }

    switch (MouseRMode) {
        case SHORT_CLICK: {
            counterr++;
            if (mouse.press_r == 0) {
                MouseRMode = NO_CLICK;
                counterr = 0;
            } else if (counterr >= 50) {
                MouseRMode = LONG_CLICK;
                counterr = 0;
            } else {
                MouseRMode = SHORT_CLICK;
            }
        }
            break;
        case LONG_CLICK: {
            if (mouse.press_r == 0) {
                MouseRMode = NO_CLICK;
            } else {
                MouseRMode = LONG_CLICK;
            }
        }
            break;
        case NO_CLICK: {
            if (mouse.press_r) {
                MouseRMode = SHORT_CLICK;
            }
        }
            break;
    }
}
