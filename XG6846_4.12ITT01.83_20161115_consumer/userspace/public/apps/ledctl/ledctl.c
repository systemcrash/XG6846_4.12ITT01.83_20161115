/***********************************************************************
 *
 *  Copyright (c) 2007-2010  Broadcom Corporation
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


#include "cms.h"
#include "cms_util.h"
#if defined(XAVI_LEDCTL)
#include "cms_boardioctl.h"

typedef struct {
    char *usr_led_name;
    BOARD_LED_NAME board_led_name;
} led_name_header;

typedef struct {
    char *usr_led_state;
    BOARD_LED_STATE board_led_state;
} led_state_header;

static led_name_header led_name_data [] = {
    {"Adsl",          kLedAdsl},
    {"SecAdsl",       kLedSecAdsl},
    {"WanData",       kLedWanData},
    {"Ses",           kLedSes},
    {"Voip",          kLedVoip},
    {"Voip1",         kLedVoip1},
    {"Voip2",         kLedVoip2},
    {"Pots",          kLedPots},
    {"Dect",          kLedDect},
    {"Gpon",          kLedGpon},
    {"MoCA",          kLedMoCA},
    {"Eth0Duplex",    kLedEth0Duplex},
    {"Eth0Spd100",    kLedEth0Spd100},
    {"Eth0Spd1000",   kLedEth0Spd1000},
    {"Eth1Duplex",    kLedEth1Duplex},
    {"Eth1Spd100",    kLedEth1Spd100},
    {"Eth1Spd1000",   kLedEth1Spd1000},
#if defined(XAVI_VI580) || defined(CONFIG_XAVI_VI580)
    {"GEth2",         kLedGe2},
    {"Eth0Spd",       kLedEth0Spd},
    {"Eth1Spd",       kLedEth1Spd},
    {"Eth2Spd",       kLedEth2Spd},
    {"Eth3Spd",       kLedEth3Spd},
#endif
#if defined(XAVI_XG6846) || defined(CONFIG_XAVI_XG6846)
    {"Power",         kLedPower},
    {"Alarm",         kLedAlarm},
#endif
#if defined(XAVI_DG150) || defined(CONFIG_XAVI_DG150)
    {"Power",         kLedPower},
    {"Eco",           kLedEco},
    {"WLANAck",       kLedWirelessAck},
#endif
    {"SIP1",          kLedSIP1},
    {"SIP2",          kLedSIP2},
    {"WanSpd100",     kLedWanSpd100},
    {"WanSpd1000",    kLedWanSpd1000},
    /* USB_LED & LAN_LED */
    {"Usb",           kLedUsb},
    {"Lan",           kLedLan},
    {NULL,            kLedEnd}
};

static led_state_header led_state_data [] = {
    {"Off",                   kLedStateOff},                   /* turn led off */
    {"On",                    kLedStateOn},                    /* turn led on */
    {"Fail",                  kLedStateFail},                  /* turn led on red */
    {"BlinkOnce",             kLedStateBlinkOnce},
    {"SlowBlinkContinues",    kLedStateSlowBlinkContinues},    /* slow blink continues at 2HZ interval */
    {"FastBlinkContinues",    kLedStateFastBlinkContinues},    /* fast blink continues at 4HZ interval */
    {"UserWpsInProgress",     kLedStateUserWpsInProgress},     /* 200ms on, 100ms off */
    {"UserWpsError",          kLedStateUserWpsError},          /* 100ms on, 100ms off */
    {"UserWpsSessionOverLap", kLedStateUserWpsSessionOverLap}, /* 100ms on, 100ms off, 5 times, off for 500ms */
    {NULL,                    kLedStateOff}
};

void usage(UINT32 exitCode)
{
    int i;

    printf("usage: \n");
#if defined(XAVI_SWITCH_DRIVER) && defined(XAVI_SWITCH_MV88E6352)
    printf("       ledctl [MVSW] [Port0|...|Port5] [Spd100|Spd1000] [On|Off|Blink]\n");
    printf("For Example: ledctl MVSW Port0 Spd100 On\n\n");
#endif
#ifdef XAVI_SFP_DDM_DEF
    printf("       ledctl [SFP_EEPROM] [A0|A2] [Data_Addrs Num]\n");
    printf("For Example: ledctl SFP_EEPROM A0 1\n\n");
#endif
    printf("       ledctl [LED_NAME] [LED_STATUE]\n");
    printf("LED_NAME : \n          ");
    for (i = 0; led_name_data[i].usr_led_name != NULL; i++) {
        printf("%s ", led_name_data[i].usr_led_name);
	(i == 0) ? : ((i % 6) ? : printf("\n          "));
    }
    printf("\nLED_STATUE : \n          ");
    for (i = 0; led_state_data[i].usr_led_state != NULL; i++) {
        printf("%s ", led_state_data[i].usr_led_state);
	(i == 0) ? : ((i % 3) ? : printf("\n          "));
    }
    printf("\n");
    exit(exitCode);
}

#if defined(XAVI_SWITCH_DRIVER) && defined(XAVI_SWITCH_MV88E6352)
void processMVSWGet(const char *led_name, const char *speed, const char *led_state)
{
    char cmd[64];
    int port = 0, led = 0, mode = 0;

    if (!cmsUtl_strncmp(led_name, "Port", 4))
        sscanf(led_name, "Port%d", &port);
    else
        usage(1);

    if (!cmsUtl_strcmp(speed, "Spd1000"))
        led = 0;
    else if (!cmsUtl_strcmp(speed, "Spd100"))
        led = 1;
    else
        usage(1);

    if (!cmsUtl_strcmp(led_state, "Off"))
        mode = 0;
    else if (!cmsUtl_strcmp(led_state, "On"))
        mode = 1;
    else if (!cmsUtl_strcmp(led_state, "Blink"))
        mode = 2;
    else
        usage(1);

    sprintf(cmd, "xavi_sw -t 30 -p %d -l %d -m %d", port, led, mode);
    system(cmd);
}
#endif

#ifdef XAVI_SFP_DDM_DEF
void processSFPEEPROMGet(const char *SFP_EEPROM_ADDR, const char *data_addrs)
{
    UINT32 ret_value = 0;

    if (!cmsUtl_strcmp(SFP_EEPROM_ADDR, "A0"))
        devCtl_getSFP_A0_EEPROM(atoi(data_addrs), &ret_value);
    else if (!cmsUtl_strcmp(SFP_EEPROM_ADDR, "A2"))
        devCtl_getSFP_A2_EEPROM(atoi(data_addrs), &ret_value);
    else
        usage(1);
    printf("The SFP Data_Addrs = %d, Ret_Value = 0x%X in 0x%s EEPROM Table!\n",
        atoi(data_addrs), ret_value, SFP_EEPROM_ADDR);
}
#endif

int main(int argc, char *argv[])
{
    int i = 0, j = 0;

    (argc > 2) ? : usage(1);

#if defined(XAVI_SWITCH_DRIVER) && defined(XAVI_SWITCH_MV88E6352)
    if (!cmsUtl_strcmp(argv[1], "MVSW"))
        processMVSWGet(argv[2], argv[3], argv[4]);
    else
#endif
#ifdef XAVI_SFP_DDM_DEF
    if (!cmsUtl_strcmp(argv[1], "SFP_EEPROM"))
        processSFPEEPROMGet(argv[2], argv[3]);
    else {
#endif

    // Add "ledctl All Off" command
    if (!cmsUtl_strcmp(argv[1], "All") && !cmsUtl_strcmp(argv[2], "Off"))
    {
	 cmsLed_Disable_All_Led();    
	 return 0;
    }

    if (!cmsUtl_strcmp(argv[1], "All") && !cmsUtl_strcmp(argv[2], "On"))
    {
	 cmsLed_Enable_All_Led();    
	 return 0;
    }

    for (i = 0; led_name_data[i].usr_led_name != NULL; i++) {
        if (!cmsUtl_strcmp(argv[1], led_name_data[i].usr_led_name)) {
            for (j = 0; led_state_data[j].usr_led_state != NULL; j++)
                if (!cmsUtl_strcmp(argv[2], led_state_data[j].usr_led_state))
                    break;
            break;
        }
    }
    ((led_name_data[i].usr_led_name == NULL) || (led_state_data[j].usr_led_state == NULL)) ?
    usage(1):
    cmsLed_Control_Process(led_name_data[i].board_led_name, led_state_data[j].board_led_state);
#ifdef XAVI_SFP_DDM_DEF
    }
#endif

    return 0;
}
#else
void usage(UINT32 exitCode)
{
   printf("usage: ledctl [WAN] [on|off|red]\n");
   printf("    WAN is the only led that is controllable by this app right now.\n");
   printf("    WAN must be specified.\n");
   printf("    one of on, off, or red must be specified.\n");

   exit(exitCode);
}


void processWanLed(const char *state)
{
   if (!cmsUtl_strcmp(state, "on"))
   {
      cmsLed_setWanConnected();
   }
   else if (!cmsUtl_strcmp(state, "off"))
   {
      cmsLed_setWanDisconnected();
   }
   else if (!cmsUtl_strcmp(state, "red"))
   {
      cmsLed_setWanFailed();
   }
   else
   {
      usage(1);
   }

}


int main(int argc, char *argv[])
{

   if (argc != 3)
   {
      usage(1);
   }

   if (!cmsUtl_strcmp(argv[1], "WAN"))
   {
      processWanLed(argv[2]);
   }
   else
   {
      usage(1);
   }

   return 0;
}
#endif
