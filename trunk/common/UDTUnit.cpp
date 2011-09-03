#include "StdAfx.h"
#include "UDTUnit.h"
#include "../common/suUtil.h"

const unsigned int g_pack_sign = 0x2C5D; //��Ĭ�ϵ�ʶ���룬��ֹ�����޷�����İ�

CUDTUnit::CUDTUnit()
{
	m_current_pos = 0;
	m_packhead_len = sizeof(UDP_DATAPACK) - UDT_DATABUF_LEN;
	m_databuf = NULL;
}

CUDTUnit::CUDTUnit(const int& key , const UDTSOCKET& udtsock , const sockaddr_storage& addr)
{
	m_key = key;
	m_udtsock = udtsock;
	m_addr = addr;	
	m_current_pos = 0;
	m_packhead_len = sizeof(UDP_DATAPACK) - UDT_DATABUF_LEN;
	m_databuf = NULL;

}


CUDTUnit::~CUDTUnit(void)
{
	if ( m_databuf )
	{
		delete m_databuf;
	}
}

PROCITEM* CUDTUnit::SubmitPack( char* data , long len )
{
	PROCITEM* p = new PROCITEM;
	p->pack_ado = new PACK_ADO;
	p->result_ado = new PACK_ADO;
	p->result_ado->data = NULL;
	p->result_ado->datalen = 0;
	p->data = CsuUtil::stream2adopack( data, len , *(p->pack_ado) );
	p->pSrvUdpPeer = this;
	p->result = NULL;
	p->sendlen = 0L;

	m_procMap.insert( PROCMAP::value_type( p , p ) );
	return p;
}

int CUDTUnit::cleanPack( PROCITEM* p )
{
	m_mutex.lock();
	PROCMAP::iterator it = m_procMap.find( p );
	if ( it != m_procMap.end() )
	{
		m_procMap.erase( it );
	}
	m_mutex.unlock();

	if( p->result_ado->data )
	{
		delete[] p->result_ado->data;
	}
	if ( p->result )
	{
		delete[] p->result;
	}

	delete p->pack_ado;
	delete p->result_ado;
	delete[] p->data;	
	delete p;

	return 0;
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

	int recv_data_len = pack->pack_len - m_packhead_len; //�յ�����Ч���ݳ���
	memcpy( m_databuf + m_current_pos , stream + m_packhead_len , recv_data_len );
	m_current_pos += recv_data_len;
	if ( m_current_pos >= pack->actual_data_len )
	{
		//���յ�һ�����������ݰ�
		SubmitPack( m_databuf , pack->actual_data_len );
		delete []m_databuf;
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
	udp_datapack.pack_flag = PACK_FLAG::FIRST_PACK;
	while ( ulRemind > 0 )
	{		
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
		udp_datapack.pack_len = m_packhead_len + data_in_pack_len;

		//�������ݰ�
		//m_packhead_len + data_in_pack_len;// ʵ��Ҫ���͵ĳ���
		UDT::send( m_udtsock , (char*)(&udp_datapack) , m_packhead_len + data_in_pack_len , 0 );
		
		udp_datapack.pack_flag = PACK_FLAG::NONFIRST_PACK;
	}
	return 0;
}

int CUDTUnit::SendPack( PROCITEM* p )
{
	CsuUtil::adopack2stream( &(p->result) , p->sendlen , *(p->result_ado) );
	SendData( p->result , p->sendlen );
	cleanPack( p );
	return 0;
}

char* CUDTUnit::GetIp()
{
	return inet_ntoa( ((sockaddr_in*)&m_addr)->sin_addr );
}

int CUDTUnit::GetPort()
{
	return  ntohs( ((sockaddr_in*)&m_addr)->sin_port ) ;
}

/*
void*  CUDTUnit::operator new( unsigned int size )
{
	//return tc_new(sizeof(CUDTUnit));
	return tc_malloc(size);
}

void CUDTUnit::operator delete( void *p )
{
	//tc_delete(p);
	tc_free(p);
}

void* CUDTUnit::operator new[]( unsigned int size)
{
	return tc_malloc(size );
}

void CUDTUnit::operator delete[]( void *p )
{
	tc_free(p);
}
*/