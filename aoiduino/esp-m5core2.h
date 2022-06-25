/******************************************************************************
**
** Copyright 2009-2022 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ARDUINO_M5STACK_Core2
#pragma once

#include "esp-esp32.h"
/**
 * @namespace AoiEsp
 * @brief Aoi esp classes.
 */
namespace AoiEsp
{
    using namespace AoiCore;
    /**
     * @class M5Core2
     * @brief Provides ESP32 M5Core2 functions.
     * @see https://github.com/m5stack/M5Core2
     *
     * This class provides ESP32 M5Core2 function.
     */
    class M5Core2 :
        public AoiEsp::Esp32
    {
    // members
    public:
        explicit M5Core2( void );
        virtual ~M5Core2( void );
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
        /* LCD */
        static String clear( StringList *args );
        static String print( StringList *args );
        static String printcr( StringList *args );
        static String println( StringList *args );
        static String setBrightness( StringList *args );
        static String setFontSize( StringList *args );
        static String sleep( StringList *args );
        static String vibrate( StringList *args );
        static String wakeup( StringList *args );
        // $ Please set your function to use.
    // static members
    private:
        static void scrollLine( void );
        static void setupScrollArea( uint16_t top, uint16_t bottom );
        static uint16_t lineHeight( void );
    // static variables
    private:
        /** Holds function table. */
        static AoiBase::FunctionTable *m_functionTable;
        /** Keep font size. */
        static uint8_t m_fontSize;
        /** Keep track of the drawing left coordinate. */
        static uint16_t m_leftCoordinate;
        /** Keep track of the drawing top coordinate */
        static uint16_t m_topCoordinate;
    };
}
#endif
