/******************************************************************************
**
** Copyright 2009-2023 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include "base-bootloader.h"
#include "base-shell.h"
/** Communication history size. */
#define HISTORY_SIZE 1+10+1
/** Loop history size. */
#define LOOP_SIZE 10+1
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
            { "class", &Shell::classes },
            { "do", &Shell::doBegin },
            { "done", &Shell::doEnd },
            { "equal", &Shell::equal },
            { "eval", &Shell::eval },
            { "get", &Shell::get },
            { "help", &Shell::help },
            { "set", &Shell::set },
            { "sh", &Shell::sh },
            { "substring", &Shell::substring },
            { "over", &Shell::over },
            { "under", &Shell::under },
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
        m_loop = new StringList[ LOOP_SIZE ];
        m_loopStarted = false;
        m_variables = "{}";
    }
    /**
     * @fn Shell::~Shell( void )
     *
     * Destructor. Member variables are deleted.
     */
    Shell::~Shell( void )
    {
        delete [] m_history;
        delete [] m_loop;
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
            if( j==_cr )
            {
                t = "";
                m_historyIndex = m_history;
                switch( interfaceType() )
                {
                    case AoiCore::_Serial:
                    case AoiCore::_BluetoothSerial:
                    app <= "";
                }
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
                        if( 0<t.length() )
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
            else if( j==_lf )
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
      * @fn void Shell::rc( void )
      *
      * Practices rc scripts from rc.0 to rc.9
      */
    void Shell::rc( void )
    {
        ClassTable *ct = loader.classTable();

    // Practice script every boards
        while( ct->pointer )
        {
            for( int n=0; n<10; n++ )
            {
            // sl is splitted by LF
                StringList *sl = ct->pointer->rcScript( STR_RCD+String(n) );
                for( int i=0; i<count(sl); i++ )
                    rc( (sl+i)->value );
                delete [] sl;
            }
            ct++;
        }
    }
    /**
      * @fn void Shell::rc( const String &args, bool debug = true )
      *
      * Practices rc scripts.
      *
      * param[in] args script.
      * param[in] measure Set true if measure mode, Otherwise set false.
      */
    void Shell::rc( const String &args, bool measure )
    {
        String s;

        if( !args.length() || !args.indexOf(STR_SHELL_COMMENT) )
            return;

        if( measure )
            debug( args );
        else
            app << args + _n;

        s = shell.practice( args );
        if( !s.length() )
            return;

        if( measure )
            debug( s );
        else
            app << s + _n;
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
     * @fn String Shell::classes( StringList *args )
     *
     * Class list.
     *
     * @param[in] args Reference to arguments.
     * @return Class list.
     */
    String Shell::classes( StringList *args )
    {
        String s;
        ClassTable *ct = 0;
        DynamicJsonBuffer json;
        JsonArray &r = json.createArray();

        switch( count(args) )
        {
            case 0:
                ct = loader.classTable();
                while( ct->pointer )
                {
                    r.add( ct->pointer->className() );
                    ct++;
                }
                r.prettyPrintTo( s );
                break;
            default:
                s = usage( "class" );
                break;
        }

        return s;
    }
    /**
     * @fn String Shell::doBegin( StringList *args )
     *
     * Start loop.
     *
     * @param[in] args Reference to arguments.
     * @return Empty String.
     */
    String Shell::doBegin( StringList *args )
    {
        String s;
        shell.m_loopStart = 0;
        shell.m_loopEnd = 0;
        shell.m_loopStep = 0;
        shell.m_loopCurrent = shell.m_loopStart;

        switch( count(args) )
        {
            case 3:
                shell.m_loopStart = _atoi( 0 );
                shell.m_loopEnd = _atoi( 1 );
                shell.m_loopStep = _atoi( 2 );
                shell.m_loopCurrent = shell.m_loopStart;
            case 0:
                delete [] shell.m_loop;
                shell.m_loop = new StringList[ LOOP_SIZE ];
                shell.m_loopStarted = true;
                break;
            default:
                s = usage( "do (start end step)?" );
                break;
        }

        return s;
    }
    /**
     * @fn String Shell::doEnd( StringList *args )
     *
     * Stop loop and Practice loop.
     *
     * @param[in] args Reference to arguments.
     * @return Empty String.
     */
    String Shell::doEnd( StringList *args )
    {
        String s, t;
        String rep = "{$i}";
        int i=0, j=0;

        switch( count(args) )
        {
            case 0:
                shell.m_loopStarted = false;
                j = count( shell.m_loop );
                while( true )
                {
                    t = (shell.m_loop+i)->value;
                    if( t=="break" )
                        break;
                    t.replace( rep, String(shell.m_loopCurrent) );
                    shell << t + _n;
                    s = shell.practice( t );
                    if( 0<s.length() )
                        shell << s + _n;
                // Next index or first
                    ::yield;
                    i++;
                    if( i==j )
                    {
                    // Set loop index
                        i = 0;
                        shell.m_loopCurrent += shell.m_loopStep;
                    }
                    if( 0<shell.m_loopStep && shell.m_loopEnd<shell.m_loopCurrent )
                        break;
                    if( 0>shell.m_loopStep && shell.m_loopEnd>shell.m_loopCurrent )
                        break;
                }
                delete [] shell.m_loop;
                shell.m_loop = new StringList[ LOOP_SIZE ];
                s = "";
                break;
            default:
                s = usage( "done" );
                break;
        }

        return s;
    }
    /**
     * @fn String Shell::equal( StringList *args )
     *
     * Check if valu1 and value2 are equal
     *
     * @param[in] args Reference to arguments.
     * @return Result string if equal, Otherwise !result string.
     */
    String Shell::equal( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 4:
                if( _a(2)==_a(3) )
                    s = prettyPrintTo( "value", _a(0) );
                else
                    s = prettyPrintTo( "value", _a(1) );
                break;
            default:
                s = usage( "equal result !result value1 value2" );
                break;
        }

        return s;
    }
    /**
     * @fn String Shell::eval( StringList *args )
     *
     * Evaluate string to shell.
     *
     * @return Evaluate result.
     */
    String Shell::eval( StringList *args )
    {
        String s, t;
        int c = count( args );

        if( c<1 )
            s = usage( "eval .+" );
        else
        {
            t = join( args, STR_SPACE );
            s = shell.practice( t );
        }

        return s;
    }
    /**
     * @fn String Shell::get( StringList *args )
     *
     * Get variable value from json.
     *
     * @param[in] args Reference to arguments.
     * @return Variable value from json.
     */
    String Shell::get( StringList *args )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.parseObject( shell.m_variables );

        switch( count(args) )
        {
            case 0:
                // all variables
                r.prettyPrintTo( s );
                break;
            case 1:
                s = prettyPrintTo( "value", static_cast<const char*>(r[_a(0)]) );
                break;
            default:
                s = usage( "get (name)" );
                break;
        }

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
      * @fn void Shell::sh( StringList *args )
      *
      * Practice sh script.
      */
    String Shell::sh( StringList *args )
    {
        String s;
        int c = count( args );

        if( c!=1 )
            s = usage( "sh script" );
        else
        {
            ClassTable *ct = loader.classTable();
        // Practice script every boards
            while( ct->pointer )
            {
            // sl is splitted by LF
                StringList *sl = ct->pointer->rcScript( _a(0) );
                for( int i=0; i<count(sl); i++ )
                    shell.rc( (sl+i)->value, false );
                delete [] sl;
                ct++;
            }
        }

        return s;
    }
    /**
     * @fn String Shell::over( StringList *args )
     *
     * Check if valu1 < value2 (over)
     *
     * @param[in] args Reference to arguments.
     * @return Result string if over, Otherwise !result string.
     */
    String Shell::over( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 4:
                if( _a(2)<_a(3) )
                    s = prettyPrintTo( "value", _a(0) );
                else
                    s = prettyPrintTo( "value", _a(1) );
                break;
            default:
                s = usage( "over result !result value1 value2" );
                break;
        }

        return s;
    }
    /**
     * @fn String Shell::set( StringList *args )
     *
     * Set variable value to json.
     *
     * @param[in] args Reference to arguments.
     * @return Set variable value.
     */
    String Shell::set( StringList *args )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.parseObject( shell.m_variables );

        switch( count(args) )
        {
            case 2:
                r[ _a(0) ] = _a( 1 );
                shell.m_variables = "";
                r.prettyPrintTo( shell.m_variables );
                s = prettyPrintTo( "value", static_cast<const char*>(r[_a(0)]) );
                break;
            default:
                s = usage( "set name value" );
                break;
        }

        return s;
    }
    /**
     * @fn String Shell::substring( StringList *args )
     *
     * Get a substring of a String.
     *
     * @param[in] args Reference to arguments.
     * @return The substring.
     */
    String Shell::substring( StringList *args )
    {
        String s, t;
        int i = 0;

        switch( count(args) )
        {
            case 3:
                i = _atoi( 0 );
                t = _a( 2 ).substring( i, i+_atoi(1) );
                s = prettyPrintTo( "value", t );
                break;
            default:
                s = usage( "substring index byte value" );
                break;
        }

        return s;
    }
    /**
     * @fn String Shell::under( StringList *args )
     *
     * Check if valu1 > value2 (over)
     *
     * @param[in] args Reference to arguments.
     * @return Result string if under, Otherwise !result string.
     */
    String Shell::under( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 4:
                if( _a(2)>_a(3) )
                    s = prettyPrintTo( "value", _a(0) );
                else
                    s = prettyPrintTo( "value", _a(1) );
                break;
            default:
                s = usage( "under result !result value1 value2" );
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
     * @fn bool Shell::doAdd( const String &args )
     *
     * Add args to loop list.
     *
     * @return Return true if args is add, Otherwise return false.
     */
    bool Shell::doAdd( const String &args )
    {
        bool b = false;
        int c = count( m_loop );

        if( args=="do" || args=="done" )
        {
        // Reserved word
        }
        else if( m_loopStarted && c<LOOP_SIZE )
        {
            (m_loop+c)->value = args;
            b = true;
        }

        return b;
    }
    /**
     * @fn bool Shell::isAdditionalArgCharacter( const String &value )
     *
     * Return true if value is additinal arg character.
     *
     * @param[in] value Charactar.
     * @return Return true if value is additional character, Otherwise return false.
     */
    bool Shell::isAdditionalArgCharacter( const String &value )
    {
        bool b = false;

        if( value==STR_SHELL_APPEND )
            b = true;
        else if( value==STR_SHELL_CREATE )
            b = true;
        else if( value==STR_SHELL_PIPE )
            b = true;
        else if( value==STR_SHELL_PIPE_ALL )
            b = true;

        return b;
    }
    /**
     * @fn bool Shell::isMethodCharacter( const String &value )
     *
     * Return true if value is method character.
     *
     * @param[in] value Charactar.
     * @return Return true if value is method character, Otherwise return false.
     */
    bool Shell::isMethodCharacter( const String &value )
    {
        bool b = false;

        if( value==STR_SHELL_APPEND )
            b = true;
        else if( value==STR_SHELL_CREATE )
            b = true;

        return b;
    }
    /**
     * @fn bool Shell::isSpecialCharacter( const String &value )
     *
     * Return true if value is special character.
     *
     * @param[in] value Charactar.
     * @return Return true if value is special character, Otherwise return false.
     */
    bool Shell::isSpecialCharacter( const String &value )
    {
        bool b = false;

        if( value==STR_SHELL_APPEND )
            b = true;
        else if( value==STR_SHELL_CREATE )
            b = true;
        else if( value==STR_SHELL_PIPE )
            b = true;
        else if( value==STR_SHELL_PIPE_ALL )
            b = true;
        else if( value==STR_SHELL_SEPARATOR )
            b = true;

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

        if( doAdd(args) )
            return s;

        StringList *sl = split( args, STR_SPACE );
        StringList *sl2 = bracket( sl, STR_SHELL_BRACKET, STR_SHELL_BRACKET );
        delete [] sl;
        sl = sl2;
        DynamicJsonBuffer json;

    // Practice functions.
        for( int i=0; i<count(sl); i++ )
        {
            t = (sl+i)->value;
            if( !isSpecialCharacter(t) )
            {
            // Reference previous result
                JsonObject &v = json.parseObject( s );
                for( JsonObject::iterator it=v.begin(); it!=v.end(); ++it )
                {
                    String rep = String("{$") + it->key + "}";
                    t.replace( rep, it->value.as<char*>() );
                }

                if( arg1.length() )
                    arg1 += STR_SPACE;
                arg1 += t;
                if( i<(count(sl)-1) )
                    continue;
            }

            bool b = false;
            StringList *sm = split( arg1+arg2, STR_SPACE );
            StringList *sm2 = bracket( sm, STR_SHELL_BRACKET, STR_SHELL_BRACKET );
            delete [] sm;
            sm = sm2;
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
        // For next practice, Set method and additional arg
            JsonObject &r = json.parseObject( s );
            arg1 = "";
            if( isMethodCharacter(t) )
                arg1 = t;
            arg2 = "";
            if( isAdditionalArgCharacter(t) )
            {
                String arg = (t==STR_SHELL_PIPE_ALL) ? s : static_cast<const char*>(r["value"]);
                if( !arg.length() )
                    continue;
                arg2 += STR_SPACE;
                if( t==STR_SHELL_PIPE_ALL )
                    arg2 += STR_SHELL_BRACKET + arg + STR_SHELL_BRACKET;
                else
                    arg2 += arg;
            }
        }
        delete [] sl;

        return s;
    }
}
