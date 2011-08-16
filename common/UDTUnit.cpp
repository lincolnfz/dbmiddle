#include "StdAfx.h"
#include "UDTUnit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const unsigned int g_pack_sign = 0x2C5D; //包默认的识别码，阻止程序无法处理的包

CUDTUnit::CUDTUnit(const int& key , const UDTSOCKET& udtsock)
{
	m_key = key;
	m_udtsock = udtsock;
	m_packhead_len = sizeof(UDP_DATAPACK) - UDT_DATABUF_LEN;
	m_current_pos = 0;
	m_databuf = NULL;

}


CUDTUnit::~CUDTUnit(void)
{
}

//分析收到的包
int CUDTUnit::AnalyzePack( const char* stream , const int stream_len )
{
	if ( stream_len < m_packhead_len ) //非法的包
	{
		return 0;
	}

	UDP_DATAPACK* pack = (UDP_DATAPACK*)stream;
	if ( pack->sign != g_pack_sign ) //判断是否是合法程序发送的数据包
	{
		return 0;
	}

	if ( pack->pack_flag == 0x01 ) //收到的是第一个数据包的数据
	{
		m_current_pos = 0;
		m_databuf = new char[pack->actual_data_len];
	}

	int recv_data_len = pack->len - m_packhead_len; //收到的有效数据长度
	memcpy( m_databuf + m_current_pos , stream + m_packhead_len , recv_data_len );
	m_current_pos += recv_data_len;
	if ( m_current_pos >= pack->actual_data_len )
	{
		//已收到一组完整的数据包
	}
	

	return 0;
}

//数据包转成网络上可以发送的流
int CUDTUnit::SendData( char*& data , unsigned long datalen )
{
	unsigned long ulRemind = datalen;
	unsigned long ulSended = 0; //已发送的长度
	int data_in_pack_len = 0; //一次包里头用户数据的长度
	UDP_DATAPACK udp_datapack;
	udp_datapack.actual_data_len = datalen;//数据实际长度
	udp_datapack.sign = g_pack_sign;
	while ( ulRemind > 0 )
	{
		udp_datapack.pack_flag = PACK_FLAG::FIRST_PACK;
		if( ulRemind >= UDT_DATABUF_LEN )
		{
			data_in_pack_len = UDT_DATABUF_LEN;			
		}
		else
		{
			data_in_pack_len = ulRemind;
		}
		memcpy( udp_datapack.data , data + ulSended , data_in_pack_len );
		ulSended += data_in_pack_len;	//以准备的字节
		ulRemind -= data_in_pack_len; //还剩下的字节
		udp_datapack.len = m_packhead_len + data_in_pack_len;

		//发送数据包
		// m_packhead_len + data_in_pack_len 实际要发送的长度
		//UDT::send( m_udtsock , (char*)(&udp_datapack) , m_packhead_len + data_in_pack_len , 0 );
		
		udp_datapack.pack_flag = PACK_FLAG::NONFIRST_PACK;
	}
	return 0;
}