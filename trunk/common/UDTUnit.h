#pragma once

#include "../udt/udt.h"

#define UDT_DATABUF_LEN  492

//udt处理单元对应peer
class CUDTUnit
{
	//udp数据定义
	typedef struct  
	{
		unsigned int sign; //识别码
		unsigned int order; //序号
		unsigned int len; //长度(头+数据) UDP包长度
		unsigned int standby; //保留
		unsigned long actual_data_len; //数据内容的实际大小
		unsigned char ptype; //0x01:正常包
		unsigned char pack_flag; //0x01开始包
		char data[UDT_DATABUF_LEN]; //数据内容
	}UDP_DATAPACK;

public:
	CUDTUnit(const int& key , const UDTSOCKET& udtsock);
	~CUDTUnit(void);

protected:
	//分析收到的包
	int AnalyzePack( char* stream , int stream_len );

	//数据包转成网络上可以发送的流
	int StreamPack( UDP_DATAPACK pack , char*& stream );

	int m_key; //标识
	UDTSOCKET m_udtsock; //sock
	int m_packhead_len; //数据包头长度

	unsigned long m_current_pos; //当前已收到的数据大小
	char* m_databuf;
};

