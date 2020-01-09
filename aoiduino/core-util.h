/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (aoiduino@gunjou.co.jp)
**
******************************************************************************/
#pragma once

#include <Arduino.h>
/** Argument utility */
#define _a(x) ((args+x)->value)
/** Argument to int. */
#define _atoi(x) AoiCore::toInt(_a(x))
/** Argument to int from hex. */
#define _atoih(x) AoiCore::toInt(_a(x),16)
/** Argument to long. */
#define _atol(x) AoiCore::toLong(_a(x))
/** Argument to long from hex. */
#define _atolh(x) AoiCore::toLong(_a(x),16)
/** Argument to unsigned int. */
#define _atoui(x) AoiCore::toUInt(_a(x))
/** Argument to unsigned int from hex. */
#define _atouih(x) AoiCore::toUInt(_a(x),16)
/** Argument to unsigned long. */
#define _atoul(x) AoiCore::toULong(_a(x))
/** Argument to unsigned long from hex. */
#define _atoulh(x) AoiCore::toULong(_a(x),16)
/** Communication new line. */
#define _n AoiCore::newLine()
/** Communication terminator. */
#define _term AoiCore::terminator()
/** Key Back space. */
#define KEY_BS String(char(0x08))+" "+String(char(0x08))
/**
 * @namespace AoiCore
 * @brief Aoi core classes.
 */
namespace AoiCore
{
    /**
     * @struct StringList.
     */
    struct StringList
    {
        /** Holds string value. */
        String value;
    };
// Utils
    int count( StringList *list );
    String debug( const String &data );
    bool isDigit( const String &data );
    String longToTime( unsigned long data );
    String newLine( void );
    StringList* split( const String &data, const String &delimiter );
    char* toCharArray( const String &data );
    String terminator( void );
    uint8_t toInt( const char data );
    int toInt( const String &data, int base = 10 );
    long toLong( const String &data, int base = 10  );
    unsigned int toUInt( const String &data, int base = 10  );
    unsigned long toULong( const String &data, int base = 10  );
    String usage( const String &usage );
    String usagesDelimiter( void );
}
