# Aoiduino
Aoi enviroment for Arduino. Provides command line interface and flexible enhancements for your product.

Dependent libraries
- ArduinoMqttClient
- ESP32Servo
- IRremoteESP8266

Supported boards are Arduino Uno, Spresense Ast, ESP32, M5Stack, M5Core2 and ESP8266. Below functions were implemented.

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

- 2020/02/02
  - Spresense Ast, Type 'help Ast' to show usage.
    - Camera    [add] cameraBegin, cameraEnd, cameraAutoWhiteBalanceMode, cameraColorEffect, cameraPictureFormat, cameraTakePicture
    - File      [add] beginUsbMsc, endUsbMsc

- 2020/09/08
  - All boards
    - [add] eval, do, done
    - [add] command separator (;), enclosed string ("), use all result in pipe (||)

  - Spresense Ast, Type 'help Ast' to show usage.
    - LTE       [add] lteHttpGet

- 2020/09/21
  - All boards
    - [add] {$key} variable for 1 line notation
  - Spresense Ast, Type 'help Ast' to show usage.
    - MQTT      [add] mqttBegin, mqttConnect, mqttPoll, mqttPublish, mqttSubscribe
    - RTC       [add] date
    - Watchdog  [add] watchdogBegin, watchdogEnd, watchdogKick, watchdogTimeleft

- 2020/12/28
  - ESP32, Type 'help Esp32' to show usage.
    - File      [add] >, >>, cat, cd, format, ll, mkdir, pwd, rm, rmdir, touch
    - LowPower  [add] deepSleep, dmesg, reboot
    - HTTP      [add] httpBegin, httpGet, httpPost
    - Watchdog  [add] watchdogBegin, watchdogEnd, watchdogKick, watchdogTimeleft
    - WiFi      [add] ifconfig, iwlist, wifiBegin, wifiEnd

- 2020/12/29
  - ESP32, Type 'help Esp32' to show usage.
    - RTC       [add] date
    - WiFi      [add] wifiRtc

- 2020/12/30
  - M5Stack, Type 'help M5Stack' to show usage.
    - LCD       [add] clear, print, printcr, println, setBrightness, setFontSize

- 2021/01/11
  - ESP8266, Type 'help Esp8266' to show usage.
    - File      [add] >, >>, cat, cd, format, ll, mkdir, pwd, rm, rmdir, touch
    - LowPower  [add] deepSleep, dmesg, reboot
    - HTTP      [add] httpBegin, httpGet, httpPost
    - RTC       [add] date
    - Watchdog  [add] watchdogBegin, watchdogEnd, watchdogKick, watchdogTimeleft
    - WiFi      [add] ifconfig, iwlist, wifiBegin, wifiEnd, wifiRtc

- 2021/01/17
  - All boards
    - [add] sh

- 2021/10/24
  - ESP8266, Type 'help Esp8266' to show usage.
    - Servo     [add] servoAttach, servoBegin, servoEnd, servoWriteMicroseconds

- 2021/11/03
  - ESP8266, Type 'help Esp8266' to show usage.
    - IR        [add] irReceiveBegin, irReceiveRaw, irSendBegin, irSendRaw
  - All boards
    - [add] equal, substring

- 2022/06/24
  - M5Core2, Type 'help M5Core2' to show usage.
    - LCD       [add] clear, print, printcr, println, setBrightness, setFontSize, sleep, wakeup

- 2022/06/25
  - M5Core2 (only M5GO_BOTTOM2), Type 'help M5Core2' to show usage.
    - LED       [add] fastLedAttach, fastLedBegin, fastLedClear, fastLedEnd, fastLedSetBrightness, fastLedShow

- 2022/06/26
  - M5Core2, Type 'help M5Core2' to show usage.
    - Serial    [add] serial

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

### Camera
```
# cameraBegin 1 30 QVGA YUV422

# cameraAutoWhiteBalanceMode AUTO

# cameraColorEffect SEPIA

# cameraPictureFormat VGA JPG

# cd /mnt/sd0

# cameraTakePicture picture.jpg
{
  "format": true,
  "height": 480,
  "size": 27520,
  "width": 640
}
# beginUsbMsc /mnt/sd0

# endUsbMsc /mnt/sd0

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

### GNSS
```
# gnssBegin 6

# gnssConfig
{
  "numSatellites": 10,
  "fixMode": "Fix",
  "dateTime": "2020-01-16T12:38:27.000565+09:00",
  "direction": 0,
  "latitude": 40.53324,
  "longitude": 141.4849,
  "velocity": 0
}
# gnssSattellites
[
  {
    "type": "GPS",
    "id": 1,
    "azimuth": 0,
    "elevation": 0,
    "signalLevel": 15.09
  },
  {
    "type": "GPS",
    "id": 7,
    "azimuth": 8,
    "elevation": 40,
    "signalLevel": 27.17
  },
  {
    "type": "GPS",
    "id": 10,
    "azimuth": 61,
    "elevation": 22,
    "signalLevel": 29.33
  },
  {
    "type": "GPS",
    "id": 11,
    "azimuth": 248,
    "elevation": 61,
    "signalLevel": 0
  },
  {
    "type": "GPS",
    "id": 16,
    "azimuth": 126,
    "elevation": 23,
    "signalLevel": 33.99
  },
  {
    "type": "GPS",
    "id": 27,
    "azimuth": 64,
    "elevation": 40,
    "signalLevel": 33.93
  },
  {
    "type": "GPS",
    "id": 30,
    "azimuth": 45,
    "elevation": 27,
    "signalLevel": 35.88
  },
  {
    "type": "Q1S",
    "id": 184,
    "azimuth": 189,
    "elevation": 58,
    "signalLevel": 39.87
  },
  {
    "type": "QCA",
    "id": 194,
    "azimuth": 189,
    "elevation": 58,
    "signalLevel": 38.06
  },
  {
    "type": "QCA",
    "id": 199,
    "azimuth": 201,
    "elevation": 40,
    "signalLevel": 30.38
  }
]
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
# lteBegin so-net.jp nuro nuro
{
  "carrier": "NTT DOCOMO",
  "firmware": "RK_02_01_01_10_41_15",
  "ipAddress": "153.140.192.26",
  "imei": "3             9",
  "signalStrength": "-94",
  "status": 4
}
```

### MQTT
```
# cd /mnt/sd0
# mqttBegin chain.pem cert.pem privkey.pem
# mqttConnect mqtt.yours.com 8883
# mqttSubscribe spresense 0
# mqttPoll
{
  "value": "Hi aoiduino!"
}
# mqttPublish spresense 0 Hi!
```

### RTC
```
# date
{
  "value": "1970-01-01T00:37:49"
}
```

### Watchdog
```
# watchdogBegin 40000
# watchdogTimeleft
{
  "value": 33237
}
# watchdogKick
# watchdogTimeleft
{
  "value": 34015
}
# watchdogEnd
```

### Redirect, pipe, and variable
```
# echo so-net.jp nuro nuro > nuro
{
  "value": "so-net.jp nuro nuro"
}
# cat nuro | lteBegin
{
  "carrier": "NTT DOCOMO",
  "firmware": "RK_02_01_01_10_41_15",
  "ipAddress": "114.169.149.246",
  "imei": "3             9",
  "signalStrength": "-81",
  "status": 4
}
# lteConfig | echo ++ {$carrier} ++
{
  "value": "++ NTT DOCOMO ++"
}
#
```

### Command separator, enclosed string and eval
```
# echo "This sentense contains | pattern." ; echo =>
{
  "value": "=>"
}
# echo "This sentense contains | pattern." | echo =>
{
  "value": "=> This sentense contains | pattern."
}
# echo "echo This sentense is evaluated by eval" | eval
{
  "value": "This sentense is evaluated by eval"
}
#
```

### Loop
```
# do 0 2 1
# echo hello {$i}
# done
echo hello 0
{
  "value": "hello 0"
}
echo hello 1
{
  "value": "hello 1"
}
echo hello 2
{
  "value": "hello 2"
}
```
