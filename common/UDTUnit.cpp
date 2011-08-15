#include "StdAfx.h"
#include "UDTUnit.h"

const unsigned int g_pack_sign = 0x2C5D; //��Ĭ�ϵ�ʶ���룬��ֹ�����޷�����İ�

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

//�����յ��İ�
int CUDTUnit::AnalyzePack( char* stream , int stream_len )
{
	if ( stream_len < m_packhead_len ) //�Ƿ��İ�
	{
		return 0;
	}

	UDP_DATAPACK* pack = (UDP_DATAPACK*)stream;
	if ( pack->sign != g_pack_sign ) //�ж��Ƿ��ǺϷ������͵����ݰ�
	{
		return 0;
	}

	if ( pack->pack_flag == 0x01 )
	{
		m_databuf = new char[pack->actual_data_len];
	}

	int recv_data_len = pack->len - m_packhead_len; //�յ�����Ч���ݳ���
	memcpy( m_databuf + m_current_pos , stream + m_packhead_len , recv_data_len );
	m_current_pos += recv_data_len;
	if ( m_current_pos == pack->actual_data_len )
	{
		//���յ�һ�����������ݰ�
	}
	

	return 0;
}

//���ݰ�ת�������Ͽ��Է��͵���
int CUDTUnit::StreamPack( UDP_DATAPACK pack , char*& stream )
{
	return 0;
}