/******************************************************************************
**
** Copyright 2009-2024 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ESP8266
#pragma once

#include "base-arduino.h"
#include "util-alexa.h"
#include "util-http.h"
#include "util-irremote.h"
#include "util-neopixel.h"
#include "util-servo.h"
// External libraries
#include <user_interface.h>
/**
 * @namespace AoiEsp
 * @brief Aoi esp classes.
 */
namespace AoiEsp
{
    using namespace AoiCore;
    /**
     * @class Esp8266
     * @brief Provides ESP8266 functions.
     * @see https://github.com/esp8266/Arduino
     *
     * This class provides ESP8266 function.
     */
    class Esp8266 :
        public AoiBase::Arduino,
        public AoiUtil::Alexa,
        public AoiUtil::Http,
        public AoiUtil::NeoPixel,
        public AoiUtil::IRRemote,
        public AoiUtil::Servo_
    {
    // members
    public:
        explicit Esp8266( void );
        virtual ~Esp8266( void );
    // members
    public:
        virtual String className( void );
        virtual bool isExist( const String &function );
        virtual String practice( StringList *args );
        virtual String usages( void );
    // members
    public:
        virtual StringList* rcScript( const String &index );
    // static members
    protected:
        // ^ Please set your function to use.
        /* File ( Flash ) */
        static String append( StringList *args );
        static String cd( StringList *args );
        static String create( StringList *args );
        static String format( StringList *args );
        static String ll( StringList *args );
        static String mkdir( StringList *args );
        static String pwd( StringList *args );
        static String read( StringList *args );
        static String remove( StringList *args );
        static String rmdir( StringList *args );
        static String touch( StringList *args );
        /* HTTP */
        static String httpBegin( StringList *args );
        static String httpGetRaw( StringList *args );
        static String httpPost( StringList *args );
        /* LowPower */
        static String deepSleep( StringList *args );
        static String dmesg( StringList *args );
        static String restart( StringList *args );
        /* RTC */
        static String date( StringList *args );
        /* Watchdog */
        static String watchdogBegin( StringList *args );
        static String watchdogEnd( StringList *args );
        static String watchdogKick( StringList *args );
        static String watchdogTimeleft( StringList *args );
        /* WiFi */
        static String ifConfig( StringList *args );
        static String wifiBegin( StringList *args );
        static String wifiEnd( StringList *args );
        static String wifiRtc( StringList *args );
        static String wifiScanNetworks( StringList *args );
        // $ Please set your function to use.
    // static members
    protected:
        static String appendRootPath( const String &path );
        static void reboot();
        static String requestBodyHeaderInPut( const String &boundary, const String &name, const String &value, int *size );
    // static variables
    private:
        /** Holds function table. */
        static AoiBase::FunctionTable *m_functionTable;
    };
}
#endif
