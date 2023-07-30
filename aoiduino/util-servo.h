/******************************************************************************
**
** Copyright 2009-2023 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#pragma once

class Servo;

#include "core-abstractcore.h"
/**
 * @namespace AoiUtil
 * @brief Aoi utility classes.
 */
namespace AoiUtil
{
    using namespace AoiCore;
    /**
     * @class Servo_
     * @brief Provides Servo functions using Arduino Servo
     *
     * This class provides Servo functions using Arduino Servo.
     */
    class Servo_
    {
    // members
    public:
        explicit Servo_( void );
        virtual ~Servo_( void );
    // static members
    protected:
        // ^ Please set your function to use.
        static String servoAttach( StringList *args );
        static String servoBegin( StringList *args );
        static String servoEnd( StringList *args );
        static String servoWriteMicroseconds( StringList *args );
        // $ Please set your function to use.
    // static variables
    private:
        /** Holds Servo */
        static Servo *m_servo;
        /** Holds Servo count */
        static int m_servoCount;
    };
}
