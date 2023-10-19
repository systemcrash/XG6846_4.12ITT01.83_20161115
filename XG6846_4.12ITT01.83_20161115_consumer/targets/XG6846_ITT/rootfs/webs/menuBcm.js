//Note: The options array initialized in menu.html follows
//      the MENU_OPTION order defined here. Both sides must
//      be in the same order.
var MENU_OPTION_USER               = 0;
var MENU_OPTION_PROTOCOL           = 2;
var MENU_OPTION_IP_EXTENSION       = 5;
var MENU_OPTION_SNMP               = 8;
var MENU_OPTION_SNTP               = 11;
var MENU_OPTION_TR69C              = 25;
var MENU_OPTION_OMCI               = 32;
var MENU_OPTION_DIAG_P8021AG       = 35;
var MENU_OPTION_OPTICAL            = 61;
var MENU_OPTION_XAVI_XMLPROV_DEF   = 62;
var MENU_OPTION_XAVI_USER_SWITCH   = 63;
var MENU_OPTION_XAVI_WAN_PROTO     = 64;
var MENU_OPTION_XAVI_LED_MODE      = 65;
var MENU_OPTION_XAVI_IGMP_SNOOPING = 66;

function menuAdmin(options) {
   var proto = options[MENU_OPTION_PROTOCOL];
   var ipExt = options[MENU_OPTION_IP_EXTENSION];
   var snmp = options[MENU_OPTION_SNMP];
   var sntp = options[MENU_OPTION_SNTP];
   var tr69c = options[MENU_OPTION_TR69C];
   var omci = options[MENU_OPTION_OMCI];
   var p8021ag = options[MENU_OPTION_DIAG_P8021AG];
   var xavi_xmlprov = options[MENU_OPTION_XAVI_XMLPROV_DEF];
   var xavi_user_switch = options[MENU_OPTION_XAVI_USER_SWITCH];
   var xavi_wan_proto = options[MENU_OPTION_XAVI_WAN_PROTO];
   var xavi_led_mode = options[MENU_OPTION_XAVI_LED_MODE];
   var xavi_igmp_snooping = options[MENU_OPTION_XAVI_IGMP_SNOOPING];

   if ( xavi_user_switch == '1' ) {
      nodeAdvancedSetup = insFld(foldersTree, gFld(getMenuTitle(MENU_XAVI_ADVANCED_SETUP), 'lancfg2.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_LAN_SETTINGS),'lancfg2.html'));
      if ( xavi_wan_proto == '1' )
         insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_WAN_SETTINGS),'wancfg.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_VLAN),'vlancfg.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_8021p_QOS),'8021pqos.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_QOS_LIST),'qoslist.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_CATV),'catvswitch.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_PORT_MODE),'portmode.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_QINQ),'qinqcfg.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_RATE_LIMITING),'ratelimitingcfg.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_PORT_POWER),'portpowercfg.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_JUMBO_MODE),'jumbocfg.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_PORT_PROTECT),'portprotect.html'));
   }
   if ( xavi_igmp_snooping == '1' )
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_IGMP),'igmpsnoopingcfg.html'));
   if ( xavi_led_mode == '1' )
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_LED_MODE),'ledcfg.html'));
  
   // Configure diagnostics menu
   nodeDiagnostics = insFld(foldersTree, gFld(getMenuTitle(MENU_DIAGNOSTICS), 'diag.html'));
   if (p8021ag == '1') {
      insDoc(nodeDiagnostics, gLnk('R', getMenuTitle(MENU_DIAGNOSTICS),'diag.html'));
      insDoc(nodeDiagnostics, gLnk('R', getMenuTitle(MENU_DIAGP8021AG),'diag8021ag.html'));
   }

   // Configure management menu
   nodeMngr = insFld(foldersTree, gFld(getMenuTitle(MENU_MANAGEMENT), 'backupsettings.html'));

   nodeSettings = insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SETTINGS), 'backupsettings.html'));
   insDoc(nodeSettings, gLnk('R', getMenuTitle(MENU_TL_SETTINGS_BACKUP),'backupsettings.html'));
   insDoc(nodeSettings, gLnk('R', getMenuTitle(MENU_TL_SETTINGS_UPDATE),'updatesettings.html'));
   insDoc(nodeSettings, gLnk('R', getMenuTitle(MENU_TL_SETTINGS_DEFAULT), 'defaultsettings.html'));
   if (xavi_xmlprov == '1')
      insDoc(nodeSettings, gLnk('R', getMenuTitle(MENU_XAVI_XMLPROV_DEF), 'xmlprov.html'));

   insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SYSTEM_LOG), 'logintro.html'));
   insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SECURITY_LOG), 'seclogintro.html'));
   if ( snmp == '1' )
      insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SNMP), 'snmpconfig.html'));
   if ( tr69c == '1' )
      insFld(nodeMngr, gFld(getMenuTitle(MENU_TR69C), 'tr69cfg.html'));
   if ( omci == '1' ) {
      nodeOmci = insFld(nodeMngr, gFld(getMenuTitle(MENU_OMCI_CFG), 'omcicfg.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_GET_SET),'omcicfg.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_CREATE),'omcicreate.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_GET_NEXT),'omcigetnext.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_MACRO),'omcimacro.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_DOWNLOAD),'omcidownload.html'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_SYSTEM),'omcisystem.html'));
   }
   if ( sntp == '1' && proto != 'Bridge' && !(proto=='PPPoE' && ipExt=='1') && !(proto=='PPPoA' && ipExt=='1') )
      insFld(nodeMngr, gFld(getMenuTitle(MENU_SNTP), 'sntpcfg.html'));

   nodeAccCntr = insFld(nodeMngr, gFld(getMenuTitle(MENU_ACC_CNTR), 'password.html'));
   insDoc(nodeAccCntr, gLnk('R', getMenuTitle(MENU_ACC_CNTR_PASSWORD), 'password.html'));
   insDoc(nodeAccCntr, gLnk('R', getMenuTitle(MENU_XAVI_ACC_CNTR_IP_ADDR), 'scacccntr.cmd?action=view'));
   insDoc(nodeAccCntr, gLnk('R', getMenuTitle(MENU_XAVI_ACC_CNTR_SERVICE), 'scsrvcntr.cmd?action=view'));

   insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_UPDATE_SOFTWARE), 'upload.html'));
   insFld(nodeMngr, gFld(getMenuTitle(MENU_RESET_ROUTER), 'resetrouter.html'));
   insFld(nodeMngr, gFld(getMenuTitle(MENU_XAVI_LOGOUT), 'logout.html'));
}

function menuSupport(options) {
   var proto = options[MENU_OPTION_PROTOCOL];
   var ipExt = options[MENU_OPTION_IP_EXTENSION];
   var snmp = options[MENU_OPTION_SNMP];
   var sntp = options[MENU_OPTION_SNTP];
   var tr69c = options[MENU_OPTION_TR69C];
   var omci = options[MENU_OPTION_OMCI];
   var xavi_xmlprov = options[MENU_OPTION_XAVI_XMLPROV_DEF];
   var xavi_user_switch = options[MENU_OPTION_XAVI_USER_SWITCH];

   if ( xavi_user_switch == '1' ) {
      nodeAdvancedSetup = insFld(foldersTree, gFld(getMenuTitle(MENU_XAVI_ADVANCED_SETUP), 'qoslist.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_QOS_LIST),'qoslist.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_CATV),'catvswitch.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_PORT_MODE),'portmode.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_RATE_LIMITING),'ratelimitingcfg.html'));
      insDoc(nodeAdvancedSetup, gLnk('R', getMenuTitle(MENU_XAVI_JUMBO_MODE),'jumbocfg.html'));
   }
  
    // Configure diagnostics menu
   nodeDiagnostics = insFld(foldersTree, gFld(getMenuTitle(MENU_DIAGNOSTICS), 'diag.html'));

   // Configure management menu
   nodeMngr = insFld(foldersTree, gFld(getMenuTitle(MENU_MANAGEMENT), 'backupsettings.html'));

   nodeSettings = insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SETTINGS), 'backupsettings.html'));
   insDoc(nodeSettings, gLnk('R', getMenuTitle(MENU_TL_SETTINGS_BACKUP),'backupsettings.html'));
   insDoc(nodeSettings, gLnk('R', getMenuTitle(MENU_TL_SETTINGS_UPDATE),'updatesettings.html'));
   insDoc(nodeSettings, gLnk('R', getMenuTitle(MENU_TL_SETTINGS_DEFAULT), 'defaultsettings.html'));
   if (xavi_xmlprov == '1')
      insDoc(nodeSettings, gLnk('R', getMenuTitle(MENU_XAVI_XMLPROV_DEF), 'xmlprov.html'));

   insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SYSTEM_LOG), 'logintro.html'));
   insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SECURITY_LOG), 'seclogintro.html'));
   if ( snmp == '1' )
      insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SNMP), 'snmpconfig.html'));
   if ( tr69c == '1' )
      insFld(nodeMngr, gFld(getMenuTitle(MENU_TR69C), 'tr69cfg.html'));
   if ( omci == '1' ) {
      nodeOmci = insFld(nodeMngr, gFld(getMenuTitle(MENU_OMCI_CFG), 'omcicfg.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_GET_SET),'omcicfg.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_CREATE),'omcicreate.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_GET_NEXT),'omcigetnext.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_MACRO),'omcimacro.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_DOWNLOAD),'omcidownload.html'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_SYSTEM),'omcisystem.html'));
   }
   if ( sntp == '1' && proto != 'Bridge' && !(proto=='PPPoE' && ipExt=='1') && !(proto=='PPPoA' && ipExt=='1') )
      insFld(nodeMngr, gFld(getMenuTitle(MENU_SNTP), 'sntpcfg.html'));

   insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_UPDATE_SOFTWARE), 'upload.html'));
   insFld(nodeMngr, gFld(getMenuTitle(MENU_RESET_ROUTER), 'resetrouter.html'));
   insFld(nodeMngr, gFld(getMenuTitle(MENU_XAVI_LOGOUT), 'logout.html'));
}

function menuUser() {
   var snmp = options[MENU_OPTION_SNMP];
   var tr69c = options[MENU_OPTION_TR69C];
   var omci = options[MENU_OPTION_OMCI];
   
   // Configure diagnostics menu
   nodeDiagnostics = insFld(foldersTree, gFld(getMenuTitle(MENU_DIAGNOSTICS), 'diag.html'));

   // Configure management menu
   nodeMngr = insFld(foldersTree, gFld(getMenuTitle(MENU_MANAGEMENT), 'logintro.html'));
   insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SYSTEM_LOG), 'logintro.html'));
   if ( snmp == '1' )
   insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_SNMP), 'snmpconfig.html'));
   if ( tr69c == '1' )
      insFld(nodeMngr, gFld(getMenuTitle(MENU_TR69C), 'tr69cfg.html'));
   if ( omci == '1' ) {
      nodeOmci = insFld(nodeMngr, gFld(getMenuTitle(MENU_OMCI_CFG), 'omcicfg.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_GET_SET),'omcicfg.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_CREATE),'omcicreate.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_GET_NEXT),'omcigetnext.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_MACRO),'omcimacro.cmd?action=view'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_DOWNLOAD),'omcidownload.html'));
      insDoc(nodeOmci, gLnk('R', getMenuTitle(MENU_OMCI_SYSTEM),'omcisystem.html'));
   }
   insFld(nodeMngr, gFld(getMenuTitle(MENU_TL_UPDATE_SOFTWARE), 'upload.html'));
   insFld(nodeMngr, gFld(getMenuTitle(MENU_XAVI_LOGOUT), 'logout.html'));
}

function createBcmMenu(options) {
   var user = options[MENU_OPTION_USER];
   var proto = options[MENU_OPTION_PROTOCOL];
   var optical = options[MENU_OPTION_OPTICAL];

   foldersTree = gFld('', 'info.html');
   // device info menu
   nodeDeviceInfo = insFld(foldersTree, gFld(getMenuTitle(MENU_DEVICE_INFO), 'info.html'));
   // device summary menu
   insFld(nodeDeviceInfo, gFld(getMenuTitle(MENU_DEVICE_SUMMARY), 'info.html'));
   nodeSts = insFld(nodeDeviceInfo, gFld(getMenuTitle(MENU_STATISTICS), 'statsifc.html'));
   insDoc(nodeSts, gLnk('R', getMenuTitle(MENU_ST_LAN), 'statsifc.html'));
   insDoc(nodeSts, gLnk('R', getMenuTitle(MENU_WAN), 'statswan.cmd'));
   if (optical == '1')
      insDoc(nodeSts, gLnk('R', getMenuTitle(MENU_OPTICAL), 'statsopticifc.html'));
   if ( user == 'admin' )
   {
      menuAdmin(options);
   }
   else if ( user == 'support' )
      menuSupport(options);
   else if ( user == 'user' )
      menuUser();
}
