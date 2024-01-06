/******************************************************************************
**
** Copyright 2009-2024 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ARDUINO_M5Stick_C
#pragma once

#include "util-irremote.h"
#include "esp-esp32.h"
/**
 * @namespace AoiEsp
 * @brief Aoi esp classes.
 */
namespace AoiEsp
{
    using namespace AoiCore;
    /**
     * @class M5Stick_C
     * @brief Provides ESP32 M5Stick-C functions.
     * @see https://docs.m5stack.com/en/core/M5Stick_C
     *
     * This class provides ESP32 M5Stick-C function.
     */
    class M5Stick_C :
        public AoiEsp::Esp32,
        public AoiUtil::IRRemote
    {
    // members
    public:
        explicit M5Stick_C( void );
        virtual ~M5Stick_C( void );
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
        static String setFontSize( StringList *args );
        static String sleep( StringList *args );
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
