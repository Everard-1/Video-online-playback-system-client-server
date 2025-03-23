#pragma once
#include "Public.h"
#include "http_parser.h"
#include <map>

class CHttpParser
{
private:
	http_parser m_parser;                 // HTTP 解析器
	http_parser_settings m_settings;      // 解析器回调函数配置
	std::map<Buffer, Buffer> m_HeaderValues; // 存储 HTTP 头部字段（键值对）
	Buffer m_status;                      // HTTP 响应状态
	Buffer m_url;                         // URL
	Buffer m_body;                        // HTTP 请求/响应体
	bool m_complete;                      // 是否解析完成
	Buffer m_lastField;                   // 临时存储上一个解析的 Header Key
public:
	CHttpParser();
	~CHttpParser();
	CHttpParser(const CHttpParser& http);
	CHttpParser& operator=(const CHttpParser& http);
public:
	size_t Parser(const Buffer& data);//解析HTTP报文
	//GET POST ... 参考http_parser.h HTTP_METHOD_MAP宏
	unsigned Method() const { return m_parser.method; }    // 获取 HTTP 方法
	const std::map<Buffer, Buffer>& Headers() { return m_HeaderValues; } // 获取 HTTP 头部
	const Buffer& Status() const { return m_status; }      // 获取状态码
	const Buffer& Url() const { return m_url; }            // 获取 URL
	const Buffer& Body() const { return m_body; }          // 获取 Body
	unsigned Errno() const { return m_parser.http_errno; } // 获取错误码
protected:
	static int OnMessageBegin(http_parser* parser);
	static int OnUrl(http_parser* parser, const char* at, size_t length);//解析URL
	static int OnStatus(http_parser* parser, const char* at, size_t length);
	//解析HTTP头部
	static int OnHeaderField(http_parser* parser, const char* at, size_t length);
	static int OnHeaderValue(http_parser* parser, const char* at, size_t length);
	static int OnHeadersComplete(http_parser* parser);
	static int OnBody(http_parser* parser, const char* at, size_t length);//解析HTTP Body
	static int OnMessageComplete(http_parser* parser);//解析完成
	int OnMessageBegin();
	int OnUrl(const char* at, size_t length);
	int OnStatus(const char* at, size_t length);
	int OnHeaderField(const char* at, size_t length);
	int OnHeaderValue(const char* at, size_t length);
	int OnHeadersComplete();
	int OnBody(const char* at, size_t length);
	int OnMessageComplete();
};

class UrlParser
{
public:
	UrlParser(const Buffer& url);
	~UrlParser() {}
	int Parser();
	Buffer operator[](const Buffer& name)const;// 获取查询参数
	Buffer Protocol()const { return m_protocol; }// 获取协议
	Buffer Host()const { return m_host; } // 获取主机
	//默认返回80
	int Port()const { return m_port; }
	void SetUrl(const Buffer& url);
	const Buffer Uri()const { return m_uri; } // 获取 URI 路径
private:
	Buffer m_url;
	Buffer m_protocol;
	Buffer m_host;
	Buffer m_uri;
	int m_port;
	std::map<Buffer, Buffer> m_values;
};