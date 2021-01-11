/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include "core-global.h"
#include "core-util.h"
using namespace AoiCore;
/*
 * Please comment out below line to use custom product template.
 */
#define _PRODUCT_TEMPLATE_
/*
 * Local functions
 */
void afterAuthentication( void );
void afterSetup( void );
void beforeAuthentication( void );
void beforeSetup( void );
/*
 * Core libraries
 */
#include "base-authentication.h"
#include "base-bootloader.h"
#include "base-shell.h"
AoiBase::BootLoader loader;
AoiBase::Shell shell;
/**
 * @fn void setup( void )
 *
 * This method is called after starting arduino.
 */
void setup()
{
    beforeSetup();
// Serial
    StringList *args1
        = split( "115200 " + String(SERIAL_8N1), " " );

    loader.begin( args1, ::_Serial );
    delete [] args1;
// Class tables is created in BootLoader. Please add your class to use.
    loader.load();
    AoiBase::ClassTable *ct = loader.classTable();
    while( ct->pointer )
    {
        debug( STR_LOADED+ct->pointer->className() );
        ct++;
    }

    afterSetup();

    debug( STR_SHELL_BOOT_1 );
    debug( STR_SHELL_BOOT_2 );
    debug( STR_SHELL_BOOT_3 );
}
/**
 * @fn void loop( void )
 *
 * This method is called while running arduino.
 */
void loop()
{
#ifdef BUILD_AUTH
    AoiBase::Authentication auth;

    while( !auth.basic(STR_LOGIN,STR_LOGIN_USER,STR_LOGIN_PASS) )
        delay( 2000 );
#endif
// Starts shell.
    shell.bash( STR_SHARP );
    delay( 2000 );
}
/**
 * @fn void beforeSetup( void )
 *
 * This method is called before setup.
 */
void beforeSetup( void )
{
    Serial.begin( 115200 );
// Waits power status.
    delay( 1000 );
}
/*
 * Product template, Please set your product below this line.
 */
#ifdef _PRODUCT_TEMPLATE_
    #include "product-template.h"
    AoiProduct::Template *product = new AoiProduct::Template;
#else
    #include "product-main.h"
    AoiProduct::Template *product = new AoiProduct::Main;
#endif
/**
 * @fn void afterSetup( void )
 *
 * This method is called after setup.
 */
void afterSetup( void )
{
    shell.rc();
    loader.end();
// Sets product template.
    debug( product->setup() );
    shell.setTemplate( product );
}
