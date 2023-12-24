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
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <unistd.h>

/* To avoid multiple define in <netinet/in.h> and <IPAddress.h> */
#ifdef INADDR_NONE
#undef INADDR_NONE
#endif

#include <IPAddress.h>

#include "spresense-wifi.h"

/* WiFi */
#include <TelitWiFi.h>
TelitWiFi wifi;

/**
* @namespace AoiSpresense
* @brief Aoi Spresense classes.
 */
namespace AoiSpresense
{
    /**
     * @fn WiFiClient::WiFiClient( void )
     *
     * Constructor. Member variables are initialized.
     */
	WiFiClient::WiFiClient( void )
	{
		m_available = 0;
		m_cid = ATCMD_INVALID_CID;
	}
    /**
     * @fn Ast::~Ast( void )
     *
     * Destructor. Member variables are deleted.
     */
	WiFiClient::~WiFiClient( void )
	{
		if( connected() )
			stop();
	}
    /**
     * @fn bool WiFiClient::begin( const String &ssid, const String &passphrase )
     *
     * Initalize GS220 and activate station mode.
     *
     * @param[in] ssid SSID to activate.
     * @param[in] passphrase Passphrase to SSID.
     * @return Return true if succeeded, Otherwise return false.
     */
	bool WiFiClient::begin( const String &ssid, const String &passphrase )
	{
		bool b = true;
        TWIFI_Params params;

		AtCmd_Init();
		Init_GS2200_SPI_type( iS110B_TypeC );
		params.mode = ATCMD_MODE_STATION;
		params.psave = ATCMD_PSAVE_DEFAULT;

		if( wifi.begin(params) )
			b = false;
		else if( wifi.activate_station(ssid,passphrase) )
			b = false;
		
		return b;
	}
    /**
     * @fn int WiFiClient::connect( IPAddress ip, uint16_t port )
     *
     * Connects to a specified IP address and port.
     *
     * @param[in] ip Server IP address.
     * @param[in] port Server port.
     * @return Return 1 if succeeded, Otherwise return 0.
     */
	int WiFiClient::connect( IPAddress ip, uint16_t port )
	{
		struct in_addr addr;
		addr.s_addr = ip;

		return connect( inet_ntoa(addr), port );
	}
    /**
     * @fn int WiFiClient::connect( const char *host, uint16_t port )
     *
     * Connects to a specified IP address and port.
     *
     * @param[in] host Server host name.
     * @param[in] port Server port.
     * @return Return 1 if succeeded, Otherwise return 0.
     */
	int WiFiClient::connect( const char *host, uint16_t port )
	{
		WiFi_InitESCBuffer();
		int i = 1;

		m_cid = wifi.connect( host, String(port) );
		if( m_cid==ATCMD_INVALID_CID )
			i = 0;

		return i;
	}
    /**
     * @fn size_t WiFiClient::write( uint8_t val )
     *
     * Send one byte data to the server the client is connected to.
     *
     * @param[in] val A value to send as a single byte.
     * @return Return 1 if succeeded, Otherwise return 0.
     */
	size_t WiFiClient::write( uint8_t val )
	{
		WiFi_InitESCBuffer();
		size_t i = 0;

		if( wifi.write(m_cid,val,1) )
			i = 1;
		m_available = 1;

		return i;
	}
    /**
     * @fn size_t WiFiClient::write( const uint8_t *buf, size_t size )
     *
     * Send series of bytes data to the server the client is connected to.
     *
     * @param[in] buf Referrence to buffer to send.
     * @param[in] size The length of the buffer.
     * @return Return number of bytes sent if succeeded, Otherwise return 0.
     */
	size_t WiFiClient::write( const uint8_t *buf, size_t size )
	{
		WiFi_InitESCBuffer();
		size_t i = 0;

		for( int j=0; j<size; j++ )
		{
			if( !wifi.write(m_cid,buf+j,1) )
				break;
			i++;
		}
		m_available = 1;

		return i;
	}
    /**
     * @fn int WiFiClient::available( void )
     *
     * Returns the number of bytes available for reading.
     *
     * @return Return 1 if succeeded, Otherwise return 0.
     */
	int WiFiClient::available( void )
	{
		return m_available;
	}
    /**
     * @fn int WiFiClient::read( void )
     *
     * Read the next byte received from the server the client is connected to.
     *
     * @return Return value if succeeded, Otherwise return -1.
     */
	int WiFiClient::read( void )
	{
		WiFi_InitESCBuffer();
		uint8_t i = 0;

		wifi.read( m_cid, &i, 1 );
		m_available = 0;

		return i;
	}
    /**
     * @fn int WiFiClient::read( uint8_t *buf, size_t size )
     *
     * Read series of bytes data from the server the client is connected to.
     *
     * @param[in/out] buf Referrence to buffer to read.
     * @param[in] size The length of the buffer.
     *
     * @return Return number of bytes read if succeeded, Otherwise return -1.
     */
	int WiFiClient::read( uint8_t *buf, size_t size )
	{
		WiFi_InitESCBuffer();
		int i = wifi.read( m_cid, buf, size );

		m_available = 0;

		return i;
	}
    /**
     * @fn int WiFiClient::peek( void )
     *
     * Returns the next byte received from the server without removing it from the buffer.
     *
     * @return Return value if succeeded, Otherwise return -1.
     */
	int WiFiClient::peek( void )
	{
		// impossible to implement
		return read();
	}
    /**
     * @fn void WiFiClient::flush( void )
     *
     * Discards any bytes that have been written to the client.
     */
	void WiFiClient::flush( void )
	{
		// impossible to implement
	}
    /**
     * @fn void WiFiClient::stop( void )
     *
     * Disconnect from the server.
     */
	void WiFiClient::stop( void )
	{
        wifi.stop( m_cid );

		m_available = 0;
		m_cid = ATCMD_INVALID_CID;
	}
    /**
     * @fn uint8_t WiFiClient::connected( void )
     *
     * Whether or not the client is connected.
     *
     * @return Return 1 if connected, Otherwise return 0.
     */
	uint8_t WiFiClient::connected( void )
	{
		uint8_t i = 0;

        if( m_available )
            i = 1;
        
        return i;
	}
    /**
     * @fn int WiFiClient::setTimeout( uint32_t milliseconds )
     *
     * Set the timeout when the client send or receive.
     *
     * @return Returns 0 if succeeded, -1 if not.
     */
	int WiFiClient::setTimeout( uint32_t milliseconds )
	{
		// impossible to implement
		return 0;
	}
}
#endif