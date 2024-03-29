#pragma once

#include "../udt/udt.h"
#include "../common/CommonHead.h"
#include "../common/Mutex.h"
#include <map>

#define UDT_DATABUF_LEN  492

class CUDTExpand;

//udt处理单元对应peer
class CUDTUnit
{
	friend class CUDTExpand;
	typedef enum _pack_flag
	{
		FIRST_PACK = 0x01,
		NONFIRST_PACK = 0x02,
	}PACK_FLAG;

	//udp数据定义
	typedef struct  
	{
		unsigned int sign; //识别码
		unsigned int order; //序号
		unsigned int pack_len; //长度(头+数据) UDP包长度
		unsigned long actual_data_len; //数据内容的实际大小
		unsigned char ptype; //0x01:正常包
		unsigned char pack_flag; //0x01开始包
		unsigned char standby1;
		unsigned char standby2;
		char data[UDT_DATABUF_LEN]; //数据内容
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

	//提交到逻辑单元处理
	virtual PROCITEM* SubmitPack( char* data , long len );

	virtual int SendPack( PROCITEM* );

	/*
	static void* operator new( unsigned int size );
	static void* operator new[]( unsigned int size);
	static void operator delete( void *p );
	static void operator delete[]( void *p );
	*/

protected:
	//分析收到的包
	int AnalyzePack( const char* stream , const int stream_len );

	//发送数据
	int SendData(  char*& data , unsigned long datalen );

	//清理packitem
	int cleanPack( PROCITEM* p );

	int m_key; //标识
	Mutex m_mutex;
	UDTSOCKET m_udtsock; //sock
	sockaddr_storage m_addr;

	int m_packhead_len; //数据包头长度	
	unsigned long m_current_pos; //当前已收到的数据大小
	char* m_databuf;
	PROCMAP m_procMap;
};

