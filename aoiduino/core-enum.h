/******************************************************************************
**
** Copyright 2009-2024 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#pragma once

/**
 * @namespace AoiCore
 * @brief Aoi core classes.
 */
namespace AoiCore
{
    /** Defines interface type */
    enum InterfaceType
    {
        _Serial = 0,
        _Ethernet,
        _WiFi,
        _BluetoothSerial
    };
}
