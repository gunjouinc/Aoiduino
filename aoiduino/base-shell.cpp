/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (aoiduino@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/masaomiura/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include "base-bootloader.h"
#include "base-shell.h"
/** Communication history size. */
#define HISTORY_SIZE 1+10+1
/** Holds boot loader. */
extern AoiBase::BootLoader loader;
/** Holds shell. */
extern AoiBase::Shell shell;
/**
 * @namespace AoiBase
 * @brief Aoi base classes.
 */
namespace AoiBase
{
// Static variables.
    AoiBase::FunctionTable *Shell::m_functionTable = 0;
    /**
     * @fn Shell::Shell( void )
     *
     * Constructor. Member variables are initialized.
     */
    Shell::Shell( void )
    {
        if( m_functionTable )
            return;
    // Sets function table, If there is no instance.
        FunctionTable ftl[] =
        {
        // ^ Please set your function to use.
            { "help", &Shell::help },
        // $ Please set your function to use.
            { "", 0 }
        };
    // Creates function table to avoid kernel panic.
        int c = sizeof(ftl) / sizeof(AoiBase::FunctionTable);
        m_functionTable = new AoiBase::FunctionTable[ c ];

        for( int i=0; i<c; i++ )
        {
            AoiBase::FunctionTable *ft = m_functionTable + i;
            ft->name = ftl[ i ].name;
            ft->pointer = ftl[ i ].pointer;
        }

        m_history = new StringList[ HISTORY_SIZE ];
        m_historyIndex = m_history;
    }
    /**
     * @fn Shell::~Shell( void )
     *
     * Destructor. Member variables are deleted.
     */
    Shell::~Shell( void )
    {
        delete [] m_history;
    }
    /**
     * @fn bool Shell::bash( const String &prompt )
     *
     * Practice bash like shell.
     *
     * @param[in] prompt Prompt.
     * @return Shell result. If shell exit normally return 0.
     */
    int Shell::bash( const String &prompt )
    {
        String s, t;
        int i = 0;

        ~app;
        app <= prompt + STR_SPACE;

        while( true )
        {
            int j = app >> NULL;
            if( j=='\r' )
            {
                t = "";
                m_historyIndex = m_history;
                if( interfaceType()==AoiCore::_Serial )
                    app <= "";
                if( s.length() )
                {
                    if( s=="exit" )
                    {
                        app << STR_LOGOUT;
                        break;
                    }
                    else
                    {
                        t = practice( s );
                        app << t + _n;
                    // Updates History. First and last history must be empty.
                        for( int k=HISTORY_SIZE-3; 0<k; k-- )
                            (m_history+k+1)->value = (m_history+k)->value;
                        (m_history+1)->value = s;
                        s = "";
                    }
                }
                app << prompt + STR_SPACE;
            }
            else if( j=='\n' )
            {
            }
            else if( j!=-1 )
            {
                if( beforeKeyInput(j,&s) )
                    afterKeyInput( j, &s );
            }
            internalLoop();
        }

        return i;
    }
    /**
     * @fn String Shell::className( void )
     *
     * @see bool AbstractBase::className( void )
     */
    String Shell::className( void )
    {
        return String( "Shell" );
    }
    /**
     * @fn bool Shell::isExist( const String &function )
     *
     * @see bool AbstractBase::isExist( const String &function )
     */
    bool Shell::isExist( const String &function )
    {
        bool b = false;
        AoiBase::FunctionTable *ft = m_functionTable;

        while( ft->pointer )
        {
            if( ft->name==function )
            {
                b = true;
                break;
            }
            ft++;
        }

        return b;
    }
    /**
     * @fn String Shell::practice( StringList *args )
     *
     * @see String AbstractBase::practice( StringList *args )
     */
    String Shell::practice( StringList *args )
    {
        String s;
        AoiBase::FunctionTable *ft = m_functionTable;

        while( ft->pointer )
        {
            if( ft->name==args->value )
            {
            // Sets argument only.
                s = (*ft->pointer)( args+1 );
                break;
            }
            ft++;
        }

        return s;
    }
    /**
     * @fn String Shell::usages( void )
     *
     * @see String AbstractBase::usages( void )
     */
    String Shell::usages( void )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        r[ "className" ] = className();

        AoiBase::FunctionTable *ft = m_functionTable;
        JsonArray &a = r.createNestedArray( "methods" );

        while( ft->pointer )
        {
            JsonObject &o = a.createNestedObject();
            o[ "name" ] = ft->name;
            o[ "usage" ] = (*ft->pointer)( 0 );
            ft++;
        }
        r.prettyPrintTo( s );

        return s;
    }
    /**
     * @fn String Shell::help( StringList *args )
     *
     * Shows help message.
     *
     * @return Empty string.
     */
    String Shell::help( StringList *args )
    {
        String s;
        ClassTable *ct = 0;
        DynamicJsonBuffer json;
        JsonArray &r = json.createArray();

        switch( count(args) )
        {
            case 1:
                ct = loader.classTable();
                s = "[" + _n;
                while( ct->pointer )
                {
                    if( ct->pointer->className()==_a(0) || String("*")==_a(0) )
                    {
                        s += ct->pointer->usages() + "," + _n;
                        r.add( ct->pointer->usages() );
                    }
                    ct++;
                }
                s.remove( s.length()-3 );
                s += _n + "]";
//                r.prettyPrintTo( s );
                break;
            default:
                s = usage( "help (classname|*)" );
                break;
        }

        return s;
    }
    /**
     * @fn void Shell::afterKeyInput( int input, String *buffer )
     *
     * @see AbstractBase::afterKeyInput( int input, String *buffer ).
     */
    void Shell::afterKeyInput( int input, String *buffer )
    {
        AbstractBase::afterKeyInput( input, buffer );
    }
    /**
     * @fn bool Shell::beforeKeyInput( int input, String *buffer )
     *
     * @see AbstractBase::beforeKeyInput( int input, String *buffer ).
     */
    bool Shell::beforeKeyInput( int input, String *buffer )
    {
        bool b = false;
        String *bf = buffer;

        m_keyCode = (m_keyCode<<8) | input;
    // VT102 key code.
        switch( m_keyCode )
        {
            case 0x08:     // BS
            case 0x7f:     // DEL
                if( 0<bf->length() )
                    b = true;
                break;
            case 0x1b:     // Esc
                break;
            case 0x1b5b:   // Esc + [ - Erase buffer.
                for( int i=0; i<bf->length(); i++ )
                    app << KEY_BS;
                *bf = "";
                break;
            case 0x1b5b41: // Up - Order history.
                *bf = (++m_historyIndex)->value;
                if( !bf->length() )
                    *bf = (--m_historyIndex)->value;
                app << *bf;
                break;
            case 0x1b5b42: // Down - Newer history.
                if( m_historyIndex != m_history )
                    m_historyIndex--;
                *bf = m_historyIndex->value;
                app << *bf;
                break;
            case 0x1b5b43: // Right
            case 0x1b5b44: // Left
                break;
            default:
                b = true;
                break;
        }
    // Resets key code, if need.
        if( m_keyCode!=0x1b && m_keyCode!=0x1b5b )
            m_keyCode = 0;

        return b;
    }
    /**
     * @fn String Shell::practice( const String &args )
     *
     * Practices functions with arguments.
     *
     * @param[in] args Some argument.
     * @return Practice result.
     */
    String Shell::practice( const String &args )
    {
        String s, t, arg1, arg2;
        StringList *sl = split( args, " " );
        DynamicJsonBuffer json;

    // Practice functions.
        for( int i=0; i<count(sl); i++ )
        {
            t = (sl+i)->value;
            if( (t!=STR_SHELL_APPEND) && (t!=STR_SHELL_CREATE) && (t!=STR_SHELL_PIPE) )
            {
                if( arg1.length() )
                    arg1 += STR_SPACE;
                arg1 += t;
                if( i<(count(sl)-1) )
                    continue;
            }

            bool b = false;
            StringList *sm = split( arg1+arg2, STR_SPACE );
            ClassTable *ct = loader.classTable();
            while( ct->pointer )
            {
                AbstractBase *ab = ct->pointer;
                if( !ab->isExist(sm->value) )
                {
                    ct++;
                    continue;
                }
                s = ab->practice( sm );
                b = true;
                break;
            }
        // Show error message if need.
            if( !b )
                s = sm->value + STR_COMMAND_NOT_FOUND;
            delete [] sm;
            if( !b )
                break;
        // For next practice.
            JsonObject &r = json.parseObject( s );
            arg1 = (t==STR_SHELL_PIPE) ? "" : t;
            arg2 = static_cast<const char*>( r["value"] );
            if( 0<arg2.length() )
                arg2 = STR_SPACE + arg2;
        }
        delete [] sl;

        return s;
    }
}
