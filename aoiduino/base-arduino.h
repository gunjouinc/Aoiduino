/******************************************************************************
**
** Copyright 2009-2024 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#pragma once

#include "base-abstractbase.h"
/**
 * @namespace AoiBase
 * @brief Aoi base classes.
 */
namespace AoiBase
{
    using namespace AoiCore;
    /**
     * @class Arduino
     * @brief Provides Arduino core functions.
     * @see https://developer.sony.com/develop/spresense/docs/arduino_developer_guide_ja.html
     *
     * This class provides Arduino core function.
     */
    class Arduino :
        public AbstractBase
    {
    // members
    public:
        explicit Arduino( void );
        virtual ~Arduino( void );
    // members
    public:
        virtual String className( void );
        virtual bool isExist( const String &function );
        virtual String practice( StringList *args );
        virtual String usages( void );
    // members
    protected:
        virtual FunctionTable* functionTable( FunctionTable *table, uint8_t size );
        virtual bool isExist( const String &function, FunctionTable *table );
        virtual String practice( StringList *args, FunctionTable *table );
        virtual String usages( FunctionTable *table );
    // static members
    protected:
        // ^ Please set your function to use.
        static String analogRead( StringList *args );
        static String analogWrite( StringList *args );
        static String attachInterrupt( StringList *args );
        static String delay( StringList *args );
        static String delayMicroseconds( StringList *args );
        static String detachInterrupt( StringList *args );
        static String digitalRead( StringList *args );
        static String digitalWrite( StringList *args );
        static String micros( StringList *args );
        static String millis( StringList *args );
        static String noTone( StringList *args );
        static String pinMode( StringList *args );
        static String tone( StringList *args );
        static String yield( StringList *args );
        static String append( StringList *args );
        static String create( StringList *args );
        // $ Please set your function to use.
    // static members
    protected:
        static bool isInterrupted( void );
    // static members
    private:
        static void interrupt( void );
    // static variables
    private:
        /** Holds function table. */
        static FunctionTable *m_functionTable;
        /** Holds interrupt state. */
        static bool m_interrupt;
    };
}
