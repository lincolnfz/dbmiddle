#pragma once

#include "../udt/udt.h"
#include "../common/CommonHead.h"
#include "../common/Mutex.h"
#include <map>

#define UDT_DATABUF_LEN  492

class CUDTExpand;

//udt����Ԫ��Ӧpeer
class CUDTUnit
{
	friend class CUDTExpand;
	typedef enum _pack_flag
	{
		FIRST_PACK = 0x01,
		NONFIRST_PACK = 0x02,
	}PACK_FLAG;

	//udp���ݶ���
	typedef struct  
	{
		unsigned int sign; //ʶ����
		unsigned int order; //���
		unsigned int pack_len; //����(ͷ+����) UDP������
		unsigned long actual_data_len; //�������ݵ�ʵ�ʴ�С
		unsigned char ptype; //0x01:������
		unsigned char pack_flag; //0x01��ʼ��
		unsigned char standby1;
		unsigned char standby2;
		char data[UDT_DATABUF_LEN]; //��������
	}UDP_DATAPACK;

	typedef std::map<LPVOID , PROCITEM*> PROCMAP;	

public:
	CUDTUnit();
	CUDTUnit(const int& key , const UDTSOCKET& udtsock , const sockaddr_storage& addr );
	~CUDTUnit(void);
	char* GetIp();
	int GetPort();
	void setkey( const int& key ){ this->m_key = key; }
	void setudtsock( const UDTSOCKET& udtsock ) { this->m_udtsock = udtsock; }
	void setaddr( const sockaddr_storage& addr ) { this->m_addr =addr; }

	//�ύ���߼���Ԫ����
	virtual PROCITEM* SubmitPack( char* data , long len );

	virtual int SendPack( PROCITEM* );

	/*
	static void* operator new( unsigned int size );
	static void* operator new[]( unsigned int size);
	static void operator delete( void *p );
	static void operator delete[]( void *p );
	*/

protected:
	//�����յ��İ�
	int AnalyzePack( const char* stream , const int stream_len );

	//��������
	int SendData(  char*& data , unsigned long datalen );

	//����packitem
	int cleanPack( PROCITEM* p );

	int m_key; //��ʶ
	Mutex m_mutex;
	UDTSOCKET m_udtsock; //sock
	sockaddr_storage m_addr;

	int m_packhead_len; //���ݰ�ͷ����	
	unsigned long m_current_pos; //��ǰ���յ������ݴ�С
	char* m_databuf;
	PROCMAP m_procMap;
};

