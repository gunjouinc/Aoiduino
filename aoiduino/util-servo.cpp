/******************************************************************************
**
** Copyright 2009-2021 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include "util-servo.h"

#include <Servo.h>

/**
* @namespace AoiUtil
* @brief Aoi utility classes.
 */
namespace AoiUtil
{
// Static variables.
    Servo *Servo_::m_servo = 0;
    int Servo_::m_servoCount = 0;
    /**
     * @fn Servo_::Servo_( void )
     *
     * Constructor. Member variables are initialized.
     */
    Servo_::Servo_( void )
    {
    }
    /**
     * @fn Servo_::~Servo_( void )
     *
     * Destructor. Member variables are deleted.
     */
    Servo_::~Servo_( void )
    {
    }
    /**
     * @fn String Servo_::servoAttach( StringList *args )
     *
     * Attach the Servo variable to a pin.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Servo_::servoAttach( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                ( m_servo+0 )->attach( _atoi(0) );
                break;
            case 2:
                ( m_servo+_atoi(1) )->attach( _atoi(0) );
                break;
            default:
                s = usage( "servoAttach pin ([0-(count-1)])" );
                break;
        }

        return s;
    }
    /**
     * @fn String Servo_::servoBegin( StringList *args )
     *
     * Initialize the servo using count.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Servo_::servoBegin( StringList *args )
    {
        String s;
        StringList sl;

        switch( count(args) )
        {
            case 1:
                servoEnd( &sl );
                m_servoCount = _atoi( 0 );
                m_servo = new Servo[ m_servoCount ];
                break;
            default:
                s = usage( "servoBegin count" );
                break;
        }

        return s;
    }
    /**
     * @fn String Servo_::servoEnd( StringList *args )
     *
     * Detach the all Servo variable from its pin.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Servo_::servoEnd( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                if( m_servo )
                {
                    for( int i=0; i<m_servoCount; i++ )
                        (m_servo+i)->detach();
                    delete [] m_servo;
                }
                m_servo = 0;
                m_servoCount = 0;
                break;
            default:
                s = usage( "servoEnd" );
                break;
        }

        return s;
    }
    /**
     * @fn String Servo_::servoWriteMicroseconds( StringList *args )
     *
     * Set microseconds to servo using pin number.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Servo_::servoWriteMicroseconds( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                ( m_servo+0 )->writeMicroseconds( _atoi(0) );
                break;
            case 2:
                ( m_servo+_atoi(1) )->writeMicroseconds( _atoi(0) );
                break;
            default:
                s = usage( "servoWriteMicroseconds micros ([0-(count-1)])" );
                break;
        }

        return s;
    }
}
