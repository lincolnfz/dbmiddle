#include "StdAfx.h"
#include "UDTUnit.h"

const unsigned int g_pack_sign = 0x2C5D; //包默认的识别码，阻止程序无法处理的包

CUDTUnit::CUDTUnit(const int& key , const UDTSOCKET& udtsock)
{
	m_key = key;
	m_udtsock = udtsock;

	UDP_DATAPACK temp;
	m_packhead_len = sizeof(UDP_DATAPACK) - sizeof(temp.data);
	m_current_pos = 0;
	m_databuf = NULL;
}


CUDTUnit::~CUDTUnit(void)
{
}

//分析收到的包
int CUDTUnit::AnalyzePack( char* stream , int stream_len )
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

	if ( pack->pack_flag == 0x01 )
	{
		m_databuf = new char[pack->actual_data_len];
	}

	int recv_data_len = pack->len - m_packhead_len; //收到的有效数据长度
	memcpy( m_databuf + m_current_pos , stream + m_packhead_len , recv_data_len );
	m_current_pos += recv_data_len;
	if ( m_current_pos == pack->actual_data_len )
	{
		//已收到一组完整的数据包
	}
	

	return 0;
}

//数据包转成网络上可以发送的流
int CUDTUnit::StreamPack( UDP_DATAPACK pack , char*& stream )
{
	return 0;
}