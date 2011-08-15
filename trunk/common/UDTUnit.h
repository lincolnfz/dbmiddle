#pragma once

#include "../udt/udt.h"

#define UDT_DATABUF_LEN  492

//udt����Ԫ��Ӧpeer
class CUDTUnit
{
	//udp���ݶ���
	typedef struct  
	{
		unsigned int sign; //ʶ����
		unsigned int order; //���
		unsigned int len; //����(ͷ+����) UDP������
		unsigned int standby; //����
		unsigned long actual_data_len; //�������ݵ�ʵ�ʴ�С
		unsigned char ptype; //0x01:������
		unsigned char pack_flag; //0x01��ʼ��
		char data[UDT_DATABUF_LEN]; //��������
	}UDP_DATAPACK;

public:
	CUDTUnit(const int& key , const UDTSOCKET& udtsock);
	~CUDTUnit(void);

protected:
	//�����յ��İ�
	int AnalyzePack( char* stream , int stream_len );

	//���ݰ�ת�������Ͽ��Է��͵���
	int StreamPack( UDP_DATAPACK pack , char*& stream );

	int m_key; //��ʶ
	UDTSOCKET m_udtsock; //sock
	int m_packhead_len; //���ݰ�ͷ����

	unsigned long m_current_pos; //��ǰ���յ������ݴ�С
	char* m_databuf;
};

