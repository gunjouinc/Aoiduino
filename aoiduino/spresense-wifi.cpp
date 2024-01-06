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
//^ LTEClient.cpp
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
//$ LTEClient.cpp
#include "spresense-wifi.h"

/* GS2200 */
#include <GS2200AtCmd.h>
#include <GS2200Hal.h>
#define CMD_TIMEOUT 10000
extern uint8_t ESCBuffer[];
extern uint32_t ESCBufferCnt;

/**
* @namespace AoiSpresense
* @brief Aoi Spresense classes.
 */
namespace AoiSpresense
{
    /**
     * @fn WiFiProvider::WiFi( void )
     *
     * Constructor. Member variables are initialized.
     */
	WiFi::WiFi( void )
	{
	}
    /**
     * @fn WiFiProvider::~WiFi( void )
     *
     * Destructor. Member variables are deleted.
     */
	WiFi::~WiFi( void )
	{
	}
    /**
     * @fn bool WiFi::attach( const String &ssid, const String &passphrase )
     *
     * Activate station mode.
     *
     * @param[in] ssid SSID to activate.
     * @param[in] passphrase Passphrase to SSID.
     * @return Return true if succeeded, Otherwise return false.
     */
	bool WiFi::attach( const String &ssid, const String &passphrase )
	{
		bool b = false;
    	uint32_t start = millis();

        // Activate station
        while( msDelta(start)<=(CMD_TIMEOUT*2) )
        {
		    if( ATCMD_RESP_OK!=AtCmd_WD() ) continue;
		    if( ATCMD_RESP_OK!=AtCmd_WPAPSK(ssid.c_str(),passphrase.c_str()) ) continue;
		    if( ATCMD_RESP_OK!=AtCmd_WA(ssid.c_str(),"",0) ) continue;

            b = true;
            break;
        }

        return b;
	}
    /**
     * @fn bool WiFi::begin( const String &ssid, const String &passphrase )
     *
     * Initalize GS220.
     *
     * @return Return true if succeeded, Otherwise return false.
     */
	bool WiFi::begin( void )
	{
		bool b = false;

		AtCmd_Init();
		Init_GS2200_SPI_type( iS110B_TypeC );

    	uint32_t start = millis();

        // Status
        while( Get_GPIO37Status() )
            AtCmd_RecvResponse();

        // Mode
        while( msDelta(start)<=CMD_TIMEOUT )
        {
		    if( ATCMD_RESP_OK!=AtCmd_AT() ) continue;

    		// Disable echo
            ATCMD_REGDOMAIN_E domain;
		    if( ATCMD_RESP_OK!=AtCmd_ATE(0) ) continue;
		    if( ATCMD_RESP_OK!=AtCmd_WREGDOMAIN_Q(&domain) ) continue;
            if( ATCMD_REGDOMAIN_TELEC!=domain )
            {
    		    if( ATCMD_RESP_OK!=AtCmd_WREGDOMAIN(ATCMD_REGDOMAIN_TELEC) )
                    continue;
            }

    		// Enable Power save mode
		    if( ATCMD_RESP_OK!=AtCmd_WRXACTIVE(ATCMD_PSAVE_DEFAULT) ) continue;
		    if( ATCMD_RESP_OK!=AtCmd_WRXPS(1) ) continue;

    		// Set Wireless mode
		    if( ATCMD_RESP_OK!=AtCmd_WM(ATCMD_MODE_STATION) ) continue;
		    if( ATCMD_RESP_OK!=AtCmd_NDHCP(1) ) continue;

    		// Bulk data mode
		    if( ATCMD_RESP_OK!=AtCmd_BDATA(1) ) continue;

            b = true;
            break;
        }

        return b;
	}
    /**
     * @fn void WiFi::end( void )
     *
     * Finalize GS220.
     */
	void WiFi::end( void )
    {
        AtCmd_RESET();
	}
    /**
     * @fn NetworkStatus WiFi::networkStatus( void )
     *
     * Return network status.
     *
     * @return Return network status.
     */
	NetworkStatus WiFi::networkStatus( void )
	{
    	ATCMD_NetworkStatus networkStatus;
        NetworkStatus ns;

        if( ATCMD_RESP_OK!=AtCmd_NSTAT(&networkStatus) )
            return ns;

        // NetworkStatus 
        ns.macAddress = networkStatus.mac;
        ns.localIP = ipv4ToString( networkStatus.addr.ipv4 );
        ns.subnetMask = ipv4ToString( networkStatus.subnet.ipv4 );
        ns.gatewayIP = ipv4ToString( networkStatus.gateway.ipv4 );
        ns.dnsIP1 = ipv4ToString( networkStatus.dns1.ipv4 );
        ns.dnsIP2 = ipv4ToString( networkStatus.dns2.ipv4 );

		return ns;
	}
    /**
     * @fn String WiFi::ipv4ToString( uint8_t *ipv4 )
     *
     * Return ipv4 to String format like 192.168.0.1.
     *
     * @return Return ipv4 String format.
     */
    String WiFi::ipv4ToString( uint8_t *ipv4 )
    {
        String s;
        String t = "%d.%d.%d.%d";

        int l = 16;
    	char *c = new char[ l ];

        memset( c, 0, l );
        sprintf( c, t.c_str(), *(ipv4+0), *(ipv4+1), *(ipv4+2), *(ipv4+3) );
        s = c;

        delete [] c;

        return s;
    }
}
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
     * @fn WiFiClient::~WiFiClient( void )
     *
     * Destructor. Member variables are deleted.
     */
	WiFiClient::~WiFiClient( void )
	{
		if( connected() )
			stop();
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
		int i = 1;

		WiFi_InitESCBuffer();

        m_cid = ATCMD_INVALID_CID;
        if( ATCMD_RESP_OK!=AtCmd_NCTCP(host,String(port).c_str(),&m_cid) )
            i = 0;
        else if( m_cid==ATCMD_INVALID_CID )
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

		if( ATCMD_RESP_OK==AtCmd_SendBulkData(m_cid,val,1) )
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

        if( ATCMD_RESP_OK==AtCmd_SendBulkData(m_cid,buf,size) )
            i = size;

		m_available = 1;

		return i;
	}
    /**
     * @fn int WiFiClient::available( void )
     *
     * Returns the number of bytes available for reading.
     *
     * @return Return bytes available if succeeded, Otherwise return 0.
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
		uint8_t i = -1;

        ATCMD_RESP_E r = AtCmd_RecvResponse();

        if( (ATCMD_RESP_BULK_DATA_RX==r) && Check_CID(m_cid) )
			memcpy( i, (ESCBuffer+1), 1 );

        m_available = ESCBufferCnt - 1;
        if( m_available<=0 )
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
		int i = -1;
        
        ATCMD_RESP_E r = AtCmd_RecvResponse();

        if( (ATCMD_RESP_BULK_DATA_RX==r) && Check_CID(m_cid) )
        {
			memcpy( buf, (ESCBuffer+1), size );
            i = size;
        }

        m_available = ESCBufferCnt - size;
        if( m_available<=0 )
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
        while( !Get_GPIO37Status() );

        while( Get_GPIO37Status() )
        {
            switch( AtCmd_RecvResponse() )
            {
                case ATCMD_RESP_BULK_DATA_RX:
                    WiFi_InitESCBuffer();
                    break;
                case ATCMD_RESP_DISCONNECT:
                    AtCmd_NCLOSE( m_cid );
                    AtCmd_NCLOSEALL();
                    WiFi_InitESCBuffer();
                    break;
            }
            delay( 10 );
        }

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