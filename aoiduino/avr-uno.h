/******************************************************************************
**
** Copyright 2009-2024 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ARDUINO_AVR_UNO
#pragma once

#include "base-arduino.h"
/**
 * @namespace AoiAvr
 * @brief Aoi Avr classes.
 */
namespace AoiAvr
{
    using namespace AoiCore;
    /**
     * @class Uno
     * @brief Provides AVR UNO functions.
     *
     * This class provides AVR UNO function.
     */
    class Uno :
        public AoiBase::Arduino
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
        // $ Please set your function to use.
    // static variables
    private:
        /** Holds function table. */
        static AoiBase::FunctionTable *m_functionTable;
    };
}
#endif
