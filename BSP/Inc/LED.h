//
// Created by admin on 2020/8/28.
//

#ifndef NEWTEST_LED_H
#define NEWTEST_LED_H

    #include <stdint.h>
    #include <stdbool.h>

    /***	MAP	***/
    #define GPIO_PORT_LED GPIOG
    /***	CONSTANT VALUE	***/

    /***	TYPE DEFINE	***/

    /***	SPECIFIC INIT CONFIGURATION	***/

    /***	EXTERNAL VARIABLES	***/

    /***	APIs	***/
    int8_t Led_add();
    int8_t Led_add_n(uint8_t num);
    int8_t Led_delete();
    int8_t Led_delete_n(uint8_t num);
    void Led_operate(uint8_t num);

#endif //NEWTEST_LED_H
