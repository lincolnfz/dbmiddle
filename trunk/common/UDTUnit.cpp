#include "StdAfx.h"
#include "UDTUnit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const unsigned int g_pack_sign = 0x2C5D; //��Ĭ�ϵ�ʶ���룬��ֹ�����޷�����İ�

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

//�����յ��İ�
int CUDTUnit::AnalyzePack( const char* stream , const int stream_len )
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

	if ( pack->pack_flag == 0x01 ) //�յ����ǵ�һ�����ݰ�������
	{
		m_current_pos = 0;
		m_databuf = new char[pack->actual_data_len];
	}

	int recv_data_len = pack->len - m_packhead_len; //�յ�����Ч���ݳ���
	memcpy( m_databuf + m_current_pos , stream + m_packhead_len , recv_data_len );
	m_current_pos += recv_data_len;
	if ( m_current_pos >= pack->actual_data_len )
	{
		//���յ�һ�����������ݰ�
	}
	

	return 0;
}

//���ݰ�ת�������Ͽ��Է��͵���
int CUDTUnit::SendData( char*& data , unsigned long datalen )
{
	unsigned long ulRemind = datalen;
	unsigned long ulSended = 0; //�ѷ��͵ĳ���
	int data_in_pack_len = 0; //һ�ΰ���ͷ�û����ݵĳ���
	UDP_DATAPACK udp_datapack;
	udp_datapack.actual_data_len = datalen;//����ʵ�ʳ���
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
		ulSended += data_in_pack_len;	//��׼�����ֽ�
		ulRemind -= data_in_pack_len; //��ʣ�µ��ֽ�
		udp_datapack.len = m_packhead_len + data_in_pack_len;

		//�������ݰ�
		// m_packhead_len + data_in_pack_len ʵ��Ҫ���͵ĳ���
		//UDT::send( m_udtsock , (char*)(&udp_datapack) , m_packhead_len + data_in_pack_len , 0 );
		
		udp_datapack.pack_flag = PACK_FLAG::NONFIRST_PACK;
	}
	return 0;
}