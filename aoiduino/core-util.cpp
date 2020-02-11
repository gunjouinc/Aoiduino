/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (aoiduino@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/masaomiura/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include "core-util.h"
#include "core-global.h"
#include "core-strings.h"

/**
 * @namespace AoiCore
 * @brief Aoi core classes.
 */
namespace AoiCore
{
    /**
     * @fn int count( StringList *list )
     *
     * Returns string list count.
     *
     * @param[in] list Refernce to string list.
     * @return String list count. If list is null, return -1.
     */
    int count( StringList *list )
    {
        int i = 0;

        if( !list )
            return -1;

        while( true )
        {
            if( (list+i)->value==NULL_STR )
                break;
            i++;
        }

        return i;
    }
    /**
     * @fn String debug( const String &data )
     *
     * Send data to debug console.
     *
     * @param[in] data Debug data.
     * @return Debug data.
     */
    String debug( const String &data )
    {
        String s = "[" + longToTime(millis()) + "] " + data;

        Serial.println( s );

        return s;
    }
    /**
     * @fn boolean isDigit( const String &data )
     *
     * Returns is data value digit.
     *
     * @param[in] data Cheched data.
     * @return If data value is digit returns true, Otherwise returns false.
     */
    bool isDigit( const String &data )
    {
        bool b = true;

        for( int i=0; i<data.length(); i++ )
        {
            if( !isdigit(data[i]) )
            {
                b = false;
                break;
            }
        }

        return b;
    }
    /**
     * @fn String longToTime( unsigned long data )
     *
     * Returns time value from long data.
     *
     * @param[in] data Long data.
     * @return Time value like M:SS.sss.
     */
    String longToTime( unsigned long data )
    {
        String s;
        String t;
    // Calcs min second msecond.
        unsigned int ul0 = data / 1000;
        unsigned int ul1 = ul0 / 60;
        unsigned int ul2 = ul0 % 60;
        unsigned int ul3 = data - (ul1*60+ul2) * 1000;
    // Formats to M:SS.sss
        t = String( ul1 );
        while( t.length()<5 )
            t = " " + t;
        s += t;
      // SSS
        t = String( ul2 );
        while( t.length()<2 )
            t = "0" + t;
        s += ":" + t;
      // sss
        t = String( ul3 );
        while( t.length()<3 )
            t = "0" + t;
        s += "." + t;

        return s;
    }
    /**
     * @fn String join( StringList *list, const String &sep, int from = 0 )
     *
     * Join string list values using separator.
     *
     * @param[in] list Refernce to string list.
     * @param[in] sep separator.
     * @param[in] from index in fist list to join.
     * @return Joined string. If argument is invalid, Return empty string.
     */
    String join( StringList *list, const String &sep, int from )
    {
        String s;
        int c = count( list );

        if( c<=from )
            return s;

        for( int i=from; i<c; i++ )
        {
            if( 0<s.length() )
                s += sep;
            s += (list+i)->value;
        }

        return s;
    }
    /**
     * @fn String newLine( void )
     *
     * Returns new line in interface.
     *
     * @return Terminator.
     */
    String newLine( void )
    {
        return "\r\n";
    }
    /**
     * @fn StringList* split( const String &data, const String &delimiter )
     *
     * Splits string by delimiter.
     *
     * @param[in] data Splitted data.
     * @param[in] delimiter Split delimiter.
     * @return Reference to string list. Please call delete [] to delete returned value.
     */
    StringList* split( const String &data, const String &delimiter )
    {
        int i = 0;
        int j = 1;
    // Gets string count.
        while( true )
        {
            i = data.indexOf( delimiter, i );
            if( i==-1 )
                break;
            i++;
            j++;
        }
    // Sets string list.
        StringList *sl = new StringList[ j + 1 ];
        i = 0;
        for( int k=0; k<j; k++ )
        {
            int l = data.indexOf( delimiter, i );
            if( l==-1 )
                (sl+k)->value = data.substring( i );
            else
                (sl+k)->value = data.substring( i, l );
            i = l + 1;
        }

        return sl;
    }
    /**
     * @fn String terminator( void )
     *
     * Returns terminator in interface.
     *
     * @return Terminator.
     */
    String terminator( void )
    {
        return "\r";
    }
    /**
     * @fn char* toCharArray( const String &data )
     *
     * Returns byte array from string.
     *
     * @param[in] data String data.
     * @return Byte array from string. Delete reference when used.
     */
    char* toCharArray( const String &data )
    {
        int i = data.length() + 1;
        char *c = new char[ i ];

        memset( c, 0, i );
        data.toCharArray( c, i );

        return c;
    }
    /**
     * @fn uint8_t toInt( const char data )
     *
     * Returns uint8_t value from char.
     *
     * @param[in] data Charactor data.
     * @return Integer value. If can't convert data, Retruns 0.
     */
    uint8_t toInt( const char data )
    {
        char i = 0;

        if( ('0'<=data) && (data<='9'))
            i = data - 0x30;
        else if( ('A'<=data) && (data<='F') )
            i = data - 0x41 + 0x0A;
        else if( ('a'<=data) && (data<='f') )
            i = data - 0x61 + 0x0A;

        return i;
    }
    /**
     * @fn int toInt( const String &data, int base = 10 )
     *
     * Returns integer value from string.
     *
     * @param[in] data String data.
     * @return Integer value. If can't convert data, Retruns 0.
     */
    int toInt( const String &data, int base )
    {
        return toLong( data, base );
    }
    /**
     * @fn int toLong( const String &data, int base = 10 )
     *
     * Returns long value from string.
     *
     * @param[in] data String data.
     * @return Long value. If can't convert data, Retruns 0.
     */
    long toLong( const String &data, int base )
    {
        int i = 20;
        char c[ i ];
    // Initializes data.
        memset( c, 0, i );
        data.toCharArray( c, i );

        return strtol( c, 0, base );
    }
    /**
     * @fn int toUInt( const String &data, int base = 10 )
     *
     * Returns unsigned integer value from string.
     *
     * @param[in] data String data.
     * @return Unsigned integer value. If can't convert data, Retruns 0.
     */
    unsigned int toUInt( const String &data, int base )
    {
        return toULong( data, base );
    }
    /**
     * @fn int toULong( const String &data, int base = 10 )
     *
     * Returns unsigned long value from string.
     *
     * @param[in] data String data.
     * @return Unsigned long value. If can't convert data, Retruns 0.
     */
    unsigned long toULong( const String &data, int base )
    {
        int i = 20;
        char c[ i ];
    // Initializes data.
        memset( c, 0, i );
        data.toCharArray( c, i );

        return strtoul( c, 0, base );
    }
    /**
     * @fn String usage( const String &usage )
     *
     * Returns usage string.
     *
     * @param[in] usage Usage to show.
     * @return Usage string.
     */
    String usage( const String &usage )
    {
        return usage;
    }
    /**
     * @fn String usagesDelimiter( void )
     *
     * Returns usages delimiter.
     *
     * @return Usages delimiter.
     */
    String usagesDelimiter( void )
    {
        return "@";
    }
}
