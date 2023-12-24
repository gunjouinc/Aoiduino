/******************************************************************************
**
** Copyright 2009-2023 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ARDUINO_spresense_ast
#pragma once

class IPAddress;

#include <Client.h>
/**
 * @namespace AoiSpresense
 * @brief Aoi Spresense classes.
 */
namespace AoiSpresense
{
	class WiFiClient :
		public Client
	{
	// members
	public:
		explicit WiFiClient();
		virtual ~WiFiClient();
	// members
	public:
		bool begin( const String &ssid, const String &passphrase );
		int connect( IPAddress ip, uint16_t port );
		int connect( const char *host, uint16_t port );
		size_t write( uint8_t val );
		size_t write( const uint8_t *buf, size_t size );
		int available( void );
		int read( void );
		int read( uint8_t *buf, size_t size );
		int peek( void );
		void flush( void );
		void stop( void );
		uint8_t connected( void );
		/**
		 * @fn operator WiFiClient:bool( void )
		 *
		 * Bool operator.
		 *
		 * @return Return true if connected, Otherwise return false.
		 */
		operator bool( void ){ return connected(); };
		int setTimeout( uint32_t milliseconds );
	// variables
        /** Holds available. */
		int m_available;
        /** Holds connection id. */
		char m_cid;
	};
}
#endif