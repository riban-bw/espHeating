/**********************************************
 * Heating Controller Firmware for ESP8266
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 **********************************************/

#include "user_main.h"
#include "esp_common.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "udhcp/dhcpd.h"

#include "uart.h"
#include "gpio.h"

#define server_ip "192.168.101.142"
#define server_port 9669

bool g_bUartEcho = true; //!@todo Probably want this false
bool g_bLed = false;
bool g_bFlash = true;

void ICACHE_FLASH_ATTR user_init(void)
{
//    //Connect to WiFi access point
//    wifi_set_opmode(STATIONAP_MODE);
//    struct station_config *config = (struct station_config *)zalloc(sizeof(struct station_config));
//    //!@todo Get WiFi configuration from EEPROM
//    sprintf(config->ssid, "waltnet");
//    sprintf(config->password, "cathjoshlucyW");
//    wifi_station_set_config(config);
//    free(config);

//    //Configure local access point
//    struct ip_info ipinfo;
//    ipinfo.gw.addr = ipaddr_addr("192.168.145.253");
//    ipinfo.ip.addr = ipaddr_addr("192.168.145.253");
//    ipinfo.netmask.addr = ipaddr_addr("255.255.255.0");
//    wifi_set_ip_info(SOFTAP_IF, &ipinfo);
//    //Configure local access point DHCP server with small lease address range to minimise memory usage
//    struct dhcp_info *pdhcp_info = NULL;
//    pdhcp_info = (struct dhcp_info *)zalloc(sizeof(struct dhcp_info));
//    pdhcp_info->start_ip = ipaddr_addr("192.168.145.101");
//    pdhcp_info->end_ip = ipaddr_addr("192.168.145.110");
//    pdhcp_info->max_leases = 10;
//    pdhcp_info->auto_time = 60;
//    pdhcp_info->decline_time = 60;
//    pdhcp_info->conflict_time = 60;
//    pdhcp_info->offer_time = 60;
//    pdhcp_info->min_lease_sec = 60;
//    //!@todo Should we set max_lease_sec? It seems to be set to 10 days within udhcpd
//    dhcp_set_info(pdhcp_info);
//    free(pdhcp_info);
//    udhcpd_start();

    //Create tasks (processes)
//    xTaskCreate(taskUart, "tsk1", 256, NULL, 2, NULL);
//    xTaskCreate(taskUdp, "tsk2", 256, NULL, 2, NULL);

//    //Configure UART
//    uart_init_new();
//    UART_SetBaudrate(BIT_RATE_9600, BIT_RATE_115200);
//    uart0_sendStr("\r\nriban Heating Controller\r\n");

    //Configure GPIO pins

    gpiSetMode(2, GPI_MODE_OUTPUT);
////    gpiSetMode(0, GPI_MODE_INPUT);
////    gpio_enable_pullup(0, false);
//    //Create timer to flash LEDs
    xTimerHandle timer = xTimerCreate("timerFlash", 100, true, 0, onTimer);
    if(timer)
        xTimerStart(timer, 0);

    uartInit(UART0, 9600, NULL);
    uartPrintln(UART0, "Heating starting");
}

void ICACHE_FLASH_ATTR onTimer(xTimerHandle pxTimer)
{
//    if(gpiRead(0))
//    if(g_bFlash)
    {
        if(g_bLed)
        {
//            gpio_set_output(0, true);
            gpiWrite(2, true);
        }
        else
        {
//            gpio_set_output(0, false);
            gpiWrite(2, false);
            uartPrintln(UART0, "Flashing LED each second");
        }
        g_bLed = !g_bLed;
    }
}

//void ICACHE_FLASH_ATTR taskUart(void *pvParameters)
//{
//    static uint8_t atHead[2];
//    static uint8_t *pCmdLine;
//    uint8_t nData;
//
//    //Get all data in Rx fifo buffer
//    while(READ_PERI_REG(UART_STATUS(UART0)) & (UART_RXFIFO_CNT << UART_RXFIFO_CNT_S))
//    {
//        wdt_feed(); //Tell watchdog that we are still here
//        nData = READ_PERI_REG(UART_FIFO(UART0)) & 0xFF;
//        if((nData != '\n') && (g_bUartEcho))
//        {
//            //!@todo implement uart read
////            uart0_tx_buffer(&nData, 1); //echo
//        }
//        //!@todo Got one byte of data now do something with it
//        if(' ' == nData)
//            g_bFlash = !g_bFlash;
//    }
//    //Check interupt status flags to see if Rx fifo full
//    if(UART_RXFIFO_FULL_INT_ST == (READ_PERI_REG(UART_INT_ST(UART0)) & UART_RXFIFO_FULL_INT_ST))
//    {
//        WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_FULL_INT_CLR);
//        //!@todo Handle Rx fifo full
//    }
//    //Check interupt status flags to see if Rx fifo TOUT??? What is TOUT?
//    else if(UART_RXFIFO_TOUT_INT_ST == (READ_PERI_REG(UART_INT_ST(UART0)) & UART_RXFIFO_TOUT_INT_ST))
//    {
//        WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_TOUT_INT_CLR);
//        //!@todo Handle Rx fifo tout
//    }
//}
