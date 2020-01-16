# Aoiduino
Aoi enviroment for Arduino. Provies command line interface and flexible enhancements for your product.

Supported boards are Arduino Uno, Spresense Ast. Below functions were implemented.

- 2020/01/09
  - All boards
    - Baudrate is 115200.
    - Command line result is json format for future release.
    - Command line help (Type 'help *' to see all functions or Type 'help className' like 'help Uno').
    - Command line history (Max 10 count), Press up or down key to view.

- 2020/01/16
  - Spresense Ast, Type 'help Ast' to show usage.
    - File      [add] cat, cd, ll, format, mkdir, pwd, rm, rmdir, touch
    - GNSS      [add] gnssBegin, gnssEnd, gnssConfig, gnssSattellites
    - Low Power [add] clockMode, coldSleep, deepSleep, dmesg, reboot, sleep
    - LTE       [add] lteBegin, lteConfig, lteEnd

## Operation example on Spresense Ast
### Low Power
```
# reboot
[    0:01.251] Loaded : Shell
[    0:01.251] Loaded : Ast
[    0:01.252] *** Welcome to aoiduino, aoi environment for arduino. ***
[    0:01.252] help * : Show all usages.
[    0:01.252] exit   : Back to login.

aoiduino login : in
password : italy

# dmesg
{
  "bootCause": 1,
  "clockMode": 0,
  "current": -4,
  "voltage": 4021
}
# 
```

### File
```
# format /mnt/spif

# mkdir 1

# echo 3 >> 2.txt
{
  "value": "3"
}
# ll
[
  {
    "type": "d",
    "name": "1",
    "size": "0"
  },
  {
    "type": "-",
    "name": "2.txt",
    "size": "1"
  }
]
# cat 2.txt
{
  "value": "3"
}
# 
```

### LTE
```
# lteBegin

# lteConfig
ERROR: Carrier name could not be obtained from the LTE network.
ERROR: PDN information could not be obtained.
ERROR: nw_stat : 0
{
  "carrier": "N/A",
  "firmware": "RK_02_01_01_10_41_15",
  "ipAddress": "0.0.0.0",
  "imei": "3             9",
  "signalStrength": "-82",
  "status": 3
}
# lteBegin line.me line@line line
{
  "carrier": "NTT DOCOMO",
  "firmware": "RK_02_01_01_10_41_15",
  "ipAddress": "153.140.192.26",
  "imei": "3             9",
  "signalStrength": "-94",
  "status": 4
}
# 
```
