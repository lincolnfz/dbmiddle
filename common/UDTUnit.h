#pragma once

#include "../udt/udt.h"

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

public:
	CUDTUnit();
	CUDTUnit(const int& key , const UDTSOCKET& udtsock , const sockaddr_storage& addr );
	~CUDTUnit(void);
	char* GetIp();
	int GetPort();

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

	int m_key; //��ʶ
	UDTSOCKET m_udtsock; //sock
	int m_packhead_len; //���ݰ�ͷ����
	sockaddr_storage m_addr;

	unsigned long m_current_pos; //��ǰ���յ������ݴ�С
	char* m_databuf;
};

