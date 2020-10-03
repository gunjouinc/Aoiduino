/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (aoiduino@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/masaomiura/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#pragma once

#include "core-abstractcore.h"
#include <ArduinoMqttClient.h>
/**
 * @namespace AoiUtil
 * @brief Aoi utility classes.
 */
namespace AoiUtil
{
    using namespace AoiCore;
    /**
     * @class Mqtt
     * @brief Provides MQTT client functions using ArduinoMqttClient.
     * @see https://github.com/arduino-libraries/ArduinoMqttClient
     *
     * This class provides MQTT client functions using ArduinoMqttClient.
     */
    class Mqtt
    {
    // members
    public:
        explicit Mqtt( void );
        virtual ~Mqtt( void );
    // static members
    protected:
        // ^ Please set your function to use.
        static String mqttConnect( StringList *args );
        static String mqttPoll( StringList *args );
        static String mqttPublish( StringList *args );
        static String mqttSubscribe( StringList *args );
        // $ Please set your function to use.
    // static variables
    protected:
        /** Holds MQTT instance. */
        static MqttClient *mqtt;
    };
}
