/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (aoiduino@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/masaomiura/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ARDUINO_spresense_ast
#pragma once

#include "base-arduino.h"
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
        public AoiBase::Arduino
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
    // static members
    protected:
        // ^ Please set your function to use.
        /* Spresense Arduino Core, Other core functions are defined in base class. */
        static String analogRead( StringList *args );
        static String led( StringList *args );
        /* File ( Flash, SDHC, eMMC ). */
        static String cd( StringList *args );
        static String ll( StringList *args );
        static String format( StringList *args );
        static String mkdir( StringList *args );
        static String pwd( StringList *args );
        static String rmdir( StringList *args );
        // $ Please set your function to use.
    // static variables
    private:
        /** Holds function table. */
        static AoiBase::FunctionTable *m_functionTable;
    };
}
#endif
