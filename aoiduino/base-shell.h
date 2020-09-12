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
        virtual void rc( const String &args );
    // members
    public:
        virtual String className( void );
        virtual bool isExist( const String &function );
        virtual String practice( StringList *args );
        virtual String usages( void );
    // static members
    protected:
        // ^ Please set your function to use.
        static String doBegin( StringList *args );
        static String doEnd( StringList *args );
        static String eval( StringList *args );
        static String help( StringList *args );
        // $ Please set your function to use.
    // members
    protected:
        virtual void afterKeyInput( int input, String *buffer );
        virtual bool beforeKeyInput( int input, String *buffer );
        virtual bool doAdd( const String &args );
        virtual bool isAdditionalArgCharacter( const String &value );
        virtual bool isMethodCharacter( const String &value );
        virtual bool isSpecialCharacter( const String &value );
        virtual String practice( const String &args );
    // variables
    private:
        /** Communication history. */
        StringList *m_history;
        /** Communication history index. */
        StringList *m_historyIndex;
        /** Loop list. */
        StringList *m_loop;
        /** Hold loop started. */
        bool m_loopStarted;
    // variables - loop index
    private:
        /** Hold loop current index. */
        int m_loopCurrent = 0;;
        /** Hold loop end index. */
        int m_loopEnd = 0;
        /** Hold loop start index. */
        int m_loopStart = 0;
        /** Hold loop step. */
        int m_loopStep = 0;
    // static variables
    private:
        /** Holds function table. */
        static AoiBase::FunctionTable *m_functionTable;
    };
}
