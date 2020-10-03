/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include "core-abstractcore.h"
/**
 * @namespace AoiCore
 * @brief Aoi core classes.
 */
namespace AoiCore
{
    /**
     * @fn AbstractCore::AbstractCore( void )
     *
     * Constructor. Member variables are initialized.
     */
    AbstractCore::AbstractCore( void )
    {
    }
    /**
     * @fn AbstractCore::~AbstractCore( void )
     *
     * Destructor. Member variables are deleted.
     */
    AbstractCore::~AbstractCore( void )
    {
    }
    /**
     * @fn int AbstractCore::bytesAvailable( int msec )
     *
     * Returns the number of bytes that are available for reading.
     *
     * @param msec Timeout for bytes available.
     * @return Number of available bytes. If there is no available bytes, returns 0.
     */
    int AbstractCore::bytesAvailable( int msec )
    {
        int i = bytesAvailable();
        int j = 0;

        while( (j<msec) && !i )
        {
            delay( 1 );
            i = bytesAvailable();
            j++;
        }

        return i;
    }
}
