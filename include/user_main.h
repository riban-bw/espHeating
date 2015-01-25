#pragma once
/**********************************************
 * Heating Controller Firmware for ESP8266
 *
 * FileName: user_main.h
 *
 * Description: header file of user application
 *
 **********************************************/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

/** @brief  RTOS task handling UART
*   @param  pvParameters
*/
void taskUart(void *pvParameters);

/** @brief  RTOS task handling UDP
*   @param  pvParameters
*/
void taskUdp(void *pvParameters);

/** @brief  Event handler for timer event
*   @param  pxTimer Timer ID
*/
void onTimer(xTimerHandle pxTimer);
