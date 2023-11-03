/******************************************************************************
**
** Copyright 2009-2023 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ARDUINO_spresense_ast
#pragma once

/* Camera */
#include <Camera.h>
/* LowPower */
#include <LowPower.h>

#include "base-arduino.h"
#include "util-http.h"
#include "util-mqtt.h"
/**
 * @namespace AoiSpresense
 * @brief Aoi Spresense classes.
 */
namespace AoiSpresense
{
    using namespace AoiCore;
    /**
     * @class Ast
     * @brief Provides Spresense Ast functions.
     * @see https://developer.sony.com/develop/spresense/docs/arduino_developer_guide_ja.html
     *
     * This class provides Spresense Ast function.
     */
    class Ast :
        public AoiBase::Arduino,
        public AoiUtil::Http,
        public AoiUtil::Mqtt
    {
    // members
    public:
        explicit Ast( void );
        virtual ~Ast( void );
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
        /* Spresense Arduino Core, Other core functions are defined in base class. */
        static String analogRead( StringList *args );
        static String led( StringList *args );
        static String pinMode( StringList *args );
        /* Camera */
        static String cameraBegin( StringList *args );
        static String cameraEnd( StringList *args );
        static String cameraSetAutoWhiteBalanceMode( StringList *args );
        static String cameraSetColorEffect( StringList *args );
        static String cameraSetStillPictureImageFormat( StringList *args );
        static String cameraTakePicture( StringList *args );
        /* File ( Flash, SDHC, eMMC ) */
        static String append( StringList *args );
        static String beginUsbMsc( StringList *args );
        static String cd( StringList *args );
        static String create( StringList *args );
        static String endUsbMsc( StringList *args );
        static String ll( StringList *args );
        static String format( StringList *args );
        static String mkdir( StringList *args );
        static String pwd( StringList *args );
        static String read( StringList *args );
        static String remove( StringList *args );
        static String rmdir( StringList *args );
        static String touch( StringList *args );
        /* GNSS */
        static String gnssBegin( StringList *args );
        static String gnssEnd( StringList *args );
        static String gnssNavData( StringList *args );
        static String gnssSattellites( StringList *args );
        static String gnssSaveEphemeris( StringList *args );
        /* LowPower */
        static String clockMode( StringList *args );
        static String coldSleep( StringList *args );
        static String current( StringList *args );
        static String deepSleep( StringList *args );
        static String dmesg( StringList *args );
        static String reboot( StringList *args );
        static String sleep( StringList *args );
        static String voltage( StringList *args );
        /* HTTP */
        static String httpBegin( StringList *args );
        static String httpPost( StringList *args );
        /* LTE */
        static String lteBegin( StringList *args );
        static String lteConfig( StringList *args );
        static String lteEnd( StringList *args );
        static String lteRtc( StringList *args );
        /* MQTT */
        static String mqttBegin( StringList *args );
        /* RTC */
        static String date( StringList *args );
        /* Watchdog */
        static String watchdogBegin( StringList *args );
        static String watchdogEnd( StringList *args );
        static String watchdogKick( StringList *args );
        static String watchdogTimeleft( StringList *args );
        // $ Please set your function to use.
    // static members
    protected:
        static bool effectFromString( const String &value, CAM_COLOR_FX *effect );
        static bool formatFromString( const String &value, CAM_IMAGE_PIX_FMT *format );
        static bool fpsFromString( const String &value, CAM_VIDEO_FPS *fps );
        static bool whiteBalanceFromString( const String &value, CAM_WHITE_BALANCE *wb );
        static String resetReason( bootcause_e bootcause );
        static bool sizeFromString( const String &value, int *width, int *height );
    // static members
    protected:
        static String requestBodyHeaderInPut( const String &boundary, const String &name, const String &value, int *size );
    // static variables
    private:
        /** Holds function table. */
        static AoiBase::FunctionTable *m_functionTable;
    };
}
#endif
