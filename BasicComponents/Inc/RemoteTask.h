/**
  ******************************************************************************
  * @FileName			    RemoteTask.h
  * @Description            Reveive the data from remote and switch the mode(FSM)
  * @author                 Steve Young
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#ifndef __REMOTETASK_H
#define __REMOTETASK_H

#include "includes.h"

//解算数据区
#define REMOTE_CONTROLLER_STICK_OFFSET  1024u
#define REMOTE_SWITCH_VALUE_UP            0x01u
#define REMOTE_SWITCH_VALUE_DOWN        0x02u
#define REMOTE_SWITCH_VALUE_CENTRAL        0x03u
#define REMOTE_SWITCH_VALUE_BUF_DEEP    16u


//¼üÊó³£Á¿Çø
#define KEY_W                0x1u
#define KEY_S                0x2u
#define KEY_A                0x4u
#define KEY_D                0x8u
#define KEY_SHIFT            0x10
#define KEY_CTRL            0x20
#define KEY_Q                0x40
#define KEY_E                0x80
#define KEY_R                0x100
#define KEY_F                0x200
#define KEY_G                0x400
#define KEY_Z                0x800
#define KEY_X                0x1000
#define KEY_C                0x2000
#define KEY_V                0x4000
#define KEY_B                0x8000


#define IGNORE_RANGE                                0

#define NORMAL_FORWARD_BACK_SPEED    600
#define NORMAL_LEFT_RIGHT_SPEED        600/1.5f
#define HIGH_FORWARD_BACK_SPEED        800
#define HIGH_LEFT_RIGHT_SPEED        800/1.5f
#define LOW_FORWARD_BACK_SPEED            300
#define LOW_LEFT_RIGHT_SPEED            300/1.5f

#define CHASSIS_TWIST_ANGLE_LIMIT        60

#define MOUSE_LR_RAMP_TICK_COUNT        50
#define MOUSR_FB_RAMP_TICK_COUNT        60

#define MOUSE_TO_YAW_ANGLE_INC_FACT        0.06//((aim_mode != 0 && find_enemy) ? 0.03f : 0.06f)
#define MOUSE_TO_PITCH_ANGLE_INC_FACT    0.06//((aim_mode != 0    && find_enemy) ? 0.03f : 0.06f)

#define SHOOTMODE_GM_ADJUST_ANGLE        0.05f


#define GATE_CLOSE    0
#define GATE_OPEN    1


typedef struct {
    int16_t ch0;
    int16_t ch1;
    int16_t ch2;
    int16_t ch3;
    int8_t s1;
    int8_t s2;
}__packed RemoteData;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
    uint8_t last_press_l;
    uint8_t last_press_r;
    uint8_t press_l;
    uint8_t press_r;
}__packed Mouse;

typedef struct {
    uint16_t v;
}__packed Key;

typedef enum {
    SHIFT,
    CTRL,
    SHIFT_CTRL,
    NO_CHANGE,
} KeyboardMode_e;

typedef enum {
    SHORT_CLICK,
    LONG_CLICK,
    NO_CLICK,
} MouseMode_e;

typedef enum {
    MoveMode_CAP_STOP_MODE,
    MoveMode_CAP_RECHARGE_MODE,
    MoveMode_CAP_RELEASE_LOW_MODE,
    MoveMode_CAP_RELEASE_HIGH_MODE,
} KeyBoard_MoveMode;

typedef struct {
    RemoteData rc;
    Mouse mouse;
    Key key;
}__packed RC_Ctl_t;

typedef enum {
    REMOTE_INPUT = 1,
    KEY_MOUSE_INPUT = 3,
    STOP = 2,
} InputMode_e;

typedef enum {
    UPPER_POS = 1,
    MIDDLE_POS = 2,
    LOWER_POS = 3,
} FunctionMode_e;

typedef struct {
    uint8_t switch_value_raw;        // the current switch value
    uint8_t switch_value1;                //  last value << 2 | value
    uint8_t switch_value2;                //
    uint8_t switch_long_value;        //keep still if no switching
    uint8_t switch_value_buf[REMOTE_SWITCH_VALUE_BUF_DEEP];
    uint8_t buf_index;
    uint8_t buf_last_index;
    uint8_t buf_end_index;
}__packed RemoteSwitch_t;

typedef enum {
    STOP_STATE = -1,
    PREPARE_STATE = 0,
    NORMAL_STATE = 1,
    ADDITIONAL_STATE_ONE = 2,
    ADDITIONAL_STATE_TWO = 3,
} WorkState_e;

typedef struct {
    int16_t rrow;
    int16_t rcol;
    int16_t lrow;
    int16_t lcol;
}__packed RemoteChannel_t;

class RemoteRecv {
private:
    uint8_t rc_data[18];
    RemoteSwitch_t g_switch1;
    uint8_t rc_first_frame;
    uint8_t rc_update;
    uint8_t rc_cnt;
    uint8_t tx_cnt;
    uint8_t tx_free;
    uint8_t rx_free;

    void GetRemoteSwitchAction(RemoteSwitch_t *sw, uint8_t val);

    void RemoteDataProcess(uint8_t *pData);

protected:
    uint16_t IWDG_counter;
    RC_Ctl_t RC_CtrlData;
    InputMode_e inputMode;
    FunctionMode_e functionMode;

public:
    RemoteRecv() {
        inputMode = REMOTE_INPUT;
        functionMode = UPPER_POS;
        rc_first_frame = 0;
        rc_update = 0;
        rc_cnt = 0;
        tx_cnt = 200;
        tx_free = 1;
        rx_free = 1;
        IWDG_counter = 0;
    }

    void Reset() {
        if (HAL_UART_Receive_DMA(&RC_UART, rc_data, 18) != HAL_OK) {
            Error_Handler();
        }
    }

    void UartRxCpltCallback() {
        rc_update = 1;
        rx_free = 1;
    }

    void RemoteDataRecv();

    inline const bool DataUpdate() {return (rc_cnt <= 17 && rc_first_frame == 1);}//rx_free == 1 &&
};

class Remote : public RemoteRecv {
private:
    uint16_t prepare_time;
    RemoteChannel_t channel;
    Mouse mouse;
    Key key;
    WorkState_e workState;
    KeyboardMode_e KeyboardMode{NO_CHANGE};
    KeyboardMode_e LastKeyboardMode{NO_CHANGE};
    MouseMode_e MouseLMode{NO_CLICK};
    MouseMode_e MouseRMode{NO_CLICK};
    KeyBoard_MoveMode KeyBoardMoveMode{MoveMode_CAP_RECHARGE_MODE};
    uint16_t KM_FORWORD_BACK_SPEED;
    uint16_t KM_LEFT_RIGHT_SPEED;



    void RemoteDataGet();

    void WorkStateFSM();

    void KeyboardModeFSM();

    void MouseModeFSM();

public:
    static Remote remote;

    Remote() {
        RemoteRecv();
        prepare_time = 0;
        channel.rrow = 0;
        channel.rcol = 0;
        channel.lrow = 0;
        channel.lcol = 0;
        workState = PREPARE_STATE;
        KM_FORWORD_BACK_SPEED = NORMAL_FORWARD_BACK_SPEED;
        KM_LEFT_RIGHT_SPEED = NORMAL_LEFT_RIGHT_SPEED;
    }

    void Handle();
    void IWDG_Handle();
    void LeverControl();

    void KeyMouseControl();

    inline const RemoteChannel_t &Channel() { return channel; }

    inline const Mouse &MouseVal() { return mouse; }

    inline const Key &KeyVal() { return key; }

    inline const WorkState_e &GetWorkState() { return workState; }

    inline const InputMode_e &GetInputMode() { return inputMode; }
};

#endif /*__ REMOTETASK_H */
