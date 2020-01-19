/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (aoiduino@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/masaomiura/Aoiduino/blob/master/LICENSE
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
     * @class Shell
     * @brief Provides shell contents.
     *
     * This class provides shell contents. This class is called after authentication
     * or need shell.
     */
    class Shell :
        public AbstractBase
    {
    // members
    public:
        explicit Shell( void );
        virtual ~Shell( void );
    // members
    public:
        virtual int bash( const String &prompt );
        virtual void rc( void );
    // members
    public:
        virtual String className( void );
        virtual bool isExist( const String &function );
        virtual String practice( StringList *args );
        virtual String usages( void );
    // static members
    protected:
        // ^ Please set your function to use.
        static String help( StringList *args );
        // $ Please set your function to use.
    // members
    protected:
        virtual void afterKeyInput( int input, String *buffer );
        virtual bool beforeKeyInput( int input, String *buffer );
        virtual String practice( const String &args );
    // variables
    private:
        /** Communication history. */
        StringList *m_history;
        /** Communication history index. */
        StringList *m_historyIndex;
    // static variables
    private:
        /** Holds function table. */
        static AoiBase::FunctionTable *m_functionTable;
    };
}
