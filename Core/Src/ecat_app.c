/*
 * ecat_app.c - EtherCAT Application callbacks
 */

#include "ecat_slv.h"
#include "utypes.h"
#include "esc.h"
#include "main.h"


// Global object instance
_Objects Obj;

//// Application callback: Read inputs from your sensors/hardware
//void cb_get_inputs(void)
//{
//    // This is called by SOES before sending data to master
//    // Update Obj.TxVar with your sensor readings
//
//    // Example: increment counter for testing
//    static uint8_t counter = 0;
//    Obj.TxVar = counter++;
//
//    // Real application example:
//    // Obj.TxVar = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
//    // Obj.TxVar = read_sensor_value();
//}
//
//// Application callback: Write outputs to your actuators/hardware
//void cb_set_outputs(void)
//{
//    // This is called by SOES after receiving data from master
//    // Use Obj.RxVar to control your outputs
//
//    // Example: control an LED
//    if (Obj.RxVar > 0) {
//        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
//    } else {
//        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//    }
//
//    // Real application examples:
//    // set_motor_speed(Obj.RxVar);
//    // HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, Obj.RxVar);
//}

// State change hook: Called when EtherCAT state changes
void pre_state_change_hook(uint8_t *as, uint8_t *an)
{
    // *as = actual state (current state)
    // *an = requested state (new state)

    // State values:
    // ESCinit   = 0x01
    // ESCpreop  = 0x02
    // ESCsafeop = 0x04
    // ESCop     = 0x08

    // Example: Initialize when entering SAFEOP
    if ((*as == ESCpreop) && (*an == ESCsafeop)) {
        // Hardware initialization
        // HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    }

    // Example: Safe shutdown when leaving OP
    if ((*as == ESCop) && (*an != ESCop)) {
        // Stop outputs for safety
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        Obj.RxVar = 0;
    }
}


