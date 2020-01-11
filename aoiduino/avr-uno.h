/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (aoiduino@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/masaomiura/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ARDUINO_AVR_UNO
#pragma once

#include "base-abstractbase.h"
/**
 * @namespace AoiAvr
 * @brief Aoi Avr classes.
 */
namespace AoiAvr
{
    using namespace AoiCore;
    /**
     * @class AnalogIO
     * @brief Provides AVR UNO functions.
     *
     * This class provides AVR UNO function.
     */
    class Uno :
        public AoiBase::AbstractBase
    {
    // members
    public:
        explicit Uno( void );
        virtual ~Uno( void );
    // members
    public:
        virtual String className( void );
        virtual bool isExist( const String &function );
        virtual String practice( StringList *args );
        virtual String usages( void );
    // static members
    protected:
        // ^ Please set your function to use.
        static String analogRead( StringList *args );
        static String analogWrite( StringList *args );
        static String delay( StringList *args );
        static String delayMicroseconds( StringList *args );
        static String digitalRead( StringList *args );
        static String digitalWrite( StringList *args );
        static String micros( StringList *args );
        static String millis( StringList *args );
        static String noTone( StringList *args );
        static String pinMode( StringList *args );
        static String tone( StringList *args );
        // $ Please set your function to use.
    // static variables
    private:
        /** Holds function table. */
        static AoiBase::FunctionTable *m_functionTable;
    };
}
#endif
