/******************************************************************************
**
** Copyright 2009-2023 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include "base-authentication.h"
/**
 * @namespace AoiBase
 * @brief Aoi base classes.
 */
namespace AoiBase
{
    /**
     * @fn Authentication::Authentication( void )
     *
     * Constructor. Member variables are initialized.
     */
    Authentication::Authentication( void )
    {
    }
    /**
     * @fn Authentication::~Authentication( void )
     *
     * Destructor. Member variables are deleted.
     */
    Authentication::~Authentication( void )
    {
    }
    /**
     * @fn bool Authentication::basic( const String &prompt, const String &user, const String &password )
     *
     * Authentication to basic login.
     *
     * @param[in] prompt Login prompt.
     * @param[in] user Login user.
     * @param[in] password Login password.
     * @return Authentication result. Return true if authentication is succeed, Otherwise returns false.
     */
    bool Authentication::basic( const String &prompt, const String &user, const String &password )
    {
        bool b = false;
        String sl, sp;
        String *s = &sl;

       ~app;
       app <= prompt + STR_SPACE;

        while( true )
        {
            int i = app >> NULL;
            if( i==_cr )
            {
                switch( interfaceType() )
                {
                    case AoiCore::_Serial:
                    case AoiCore::_BluetoothSerial:
                    app <= "";
                }
            // Authorizes user.
                if( sl.length() && sp.length() )
                {
                    if( sl==user && sp==password )
                        b = true;
                    else
                        app << STR_INCORRECT;
                    break;
                }
            // Shows login prompt.
                if( !sl.length() && !sp.length() )
                {
                    app << prompt + STR_SPACE;
                    s = &sl;
                }
                else if( sl.length() && !sp.length() )
                {
                    app << STR_PASSWORD;
                    s = &sp;
                }
            }
            else if( i==_lf )
            {}
            else if( i!=-1 )
            {
                if( beforeKeyInput(i,s) )
                    afterKeyInput( i, s );
            }
            internalLoop();
        }

        return b;
    }
    /**
     * @fn String Authentication::className( void )
     *
     * @see bool AbstractBase::className( void )
     */
    String Authentication::className( void )
    {
        return String( "Authentication" );
    }
    /**
     * @fn bool Authentication::isExist( const String &function )
     *
     * @see bool AbstractBase::isExist( const String &function )
     */
    bool Authentication::isExist( const String &function )
    {
        return false;
    }
    /**
     * @fn String Authentication::practice( StringList *args )
     *
     * @see String AbstractBase::practice( StringList *args )
     */
    String Authentication::practice( StringList *args )
    {
        return String();
    }
    /**
     * @fn String Authentication::usages( void )
     *
     * @see String AbstractBase::usages( void )
     */
    String Authentication::usages( void )
    {
        return String();
    }
}
