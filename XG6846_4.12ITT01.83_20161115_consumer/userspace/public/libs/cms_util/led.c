/***********************************************************************
 *
 *  Copyright (c) 2006-2007  Broadcom Corporation
 *  All Rights Reserved
 *
<:label-BRCM:2012:DUAL/GPL:standard

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:>
 *
 ************************************************************************/

#ifdef XAVI_SIP_STACK
#ifdef __cplusplus
extern "C" {
#endif
#endif

#include "cms.h"
#include "cms_util.h"
#include "cms_boardioctl.h"

#if defined(XAVI_LEDCTL)
#if defined(XAVI_VI580) || defined(XAVI_FI560) || defined(XAVI_DG150)
extern int prctl_runCommandInShellWithTimeout(char *command);
#endif
#endif

/*
 * See:
 * bcmdrivers/opensource/include/bcm963xx/board.h
 * bcmdrivers/opensource/char/board/bcm963xx/impl1/board.c and bcm63xx_led.c
 */

void cmsLed_setWanConnected(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedWanData, kLedStateOn, NULL);
}

void cmsLed_setWanDisconnected(void)
{
	devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedWanData, kLedStateOff, NULL);
}


void cmsLed_setWanFailed(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedWanData, kLedStateFail, NULL);  
}


#if defined(XAVI_LEDCTL)
#if defined(XAVI_SIP_STACK)
void cmsLed_setSIP1SlowBlink(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedSIP1, kLedStateSlowBlinkContinues, NULL);
}

void cmsLed_setSIP1Blink(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedSIP1, kLedStateFastBlinkContinues, NULL);
}

void cmsLed_setSIP2SlowBlink(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedSIP2, kLedStateSlowBlinkContinues, NULL);
}

void cmsLed_setSIP2Blink(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedSIP2, kLedStateFastBlinkContinues, NULL);
}

void cmsLed_setSIP1On(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedSIP1, kLedStateOn, NULL);
}

void cmsLed_setSIP2On(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedSIP2, kLedStateOn, NULL);
}

void cmsLed_setSIP1Off(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedSIP1, kLedStateOff, NULL);
}

void cmsLed_setSIP2Off(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedSIP2, kLedStateOff, NULL);
}
#endif

void cmsLed_Control_Process(BOARD_LED_NAME led_name, BOARD_LED_STATE led_state)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, led_name, led_state, NULL);
}


void cmsLed_Disable_All_Led(void)
{
   /* disable board leds */
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL_ALL_OFF, 0, NULL, 0, 0, NULL);

   /* disable wifi led */   
#if defined(XAVI_VI580) || defined(XAVI_FI560)
   prctl_runCommandInShellWithTimeout("wl ledbh 1 0\n");
#elif  defined(XAVI_DG150)
   prctl_runCommandInShellWithTimeout("wl ledbh 0 1\n");
#endif

   /* disable ext phy leds */
#if defined (XAVI_VI580)
   /* (bcm5481) */
   prctl_runCommandInShellWithTimeout("ethctl phy ext 0x12 0x1C 0xB4EE\n"); //disable LED2. LED1
   prctl_runCommandInShellWithTimeout("ethctl phy ext 0x11 0x1C 0xB4EE\n"); //disable LED2. LED1
#elif defined (XAVI_FI560) || defined(XAVI_DG150)
   /* (bcm54610) || (bcm54612)*/
   prctl_runCommandInShellWithTimeout("ethctl phy ext 0x18 0x1C 0xB41E\n"); //disable LED1
   prctl_runCommandInShellWithTimeout("ethctl phy ext 0x18 0x1C 0xB86E\n"); //disable LED3   
   prctl_runCommandInShellWithTimeout("ethctl phy ext 0x19 0x1C 0xB41E\n"); //disable LED1
   prctl_runCommandInShellWithTimeout("ethctl phy ext 0x19 0x1C 0xB86E\n"); //disable LED3   
#endif

}

void cmsLed_Enable_All_Led(void)
{
   /* Enable board leds */
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL_ALL_ON, 0, NULL, 0, 0, NULL);
   
#if defined(XAVI_VI580) || defined(XAVI_FI560) || defined(XAVI_DG150)
   /* Enable wifi leds */
   // prctl_runCommandInShellWithTimeout("wl ledbh 1 1\n");
#endif
#if defined (XAVI_VI580)
   /* disable ext phy leds (bcm5481) */
   // prctl_runCommandInShellWithTimeout("ethctl phy ext 0x12 0x1C 0xB4EE\n");
   // prctl_runCommandInShellWithTimeout("ethctl phy ext 0x11 0x1C 0xB4EE\n");
#endif
}
#endif

#ifdef XAVI_SIP_STACK
#ifdef __cplusplus
}
#endif
#endif
