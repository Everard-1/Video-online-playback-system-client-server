#pragma once
#include "Public.h"
#include "http_parser.h"
#include <map>

class CHttpParser
{
private:
	http_parser m_parser;                 // HTTP ������
	http_parser_settings m_settings;      // �������ص���������
	std::map<Buffer, Buffer> m_HeaderValues; // �洢 HTTP ͷ���ֶΣ���ֵ�ԣ�
	Buffer m_status;                      // HTTP ��Ӧ״̬
	Buffer m_url;                         // URL
	Buffer m_body;                        // HTTP ����/��Ӧ��
	bool m_complete;                      // �Ƿ�������
	Buffer m_lastField;                   // ��ʱ�洢��һ�������� Header Key
public:
	CHttpParser();
	~CHttpParser();
	CHttpParser(const CHttpParser& http);
	CHttpParser& operator=(const CHttpParser& http);
public:
	size_t Parser(const Buffer& data);//����HTTP����
	//GET POST ... �ο�http_parser.h HTTP_METHOD_MAP��
	unsigned Method() const { return m_parser.method; }    // ��ȡ HTTP ����
	const std::map<Buffer, Buffer>& Headers() { return m_HeaderValues; } // ��ȡ HTTP ͷ��
	const Buffer& Status() const { return m_status; }      // ��ȡ״̬��
	const Buffer& Url() const { return m_url; }            // ��ȡ URL
	const Buffer& Body() const { return m_body; }          // ��ȡ Body
	unsigned Errno() const { return m_parser.http_errno; } // ��ȡ������
protected:
	static int OnMessageBegin(http_parser* parser);
	static int OnUrl(http_parser* parser, const char* at, size_t length);//����URL
	static int OnStatus(http_parser* parser, const char* at, size_t length);
	//����HTTPͷ��
	static int OnHeaderField(http_parser* parser, const char* at, size_t length);
	static int OnHeaderValue(http_parser* parser, const char* at, size_t length);
	static int OnHeadersComplete(http_parser* parser);
	static int OnBody(http_parser* parser, const char* at, size_t length);//����HTTP Body
	static int OnMessageComplete(http_parser* parser);//�������
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
	Buffer operator[](const Buffer& name)const;// ��ȡ��ѯ����
	Buffer Protocol()const { return m_protocol; }// ��ȡЭ��
	Buffer Host()const { return m_host; } // ��ȡ����
	//Ĭ�Ϸ���80
	int Port()const { return m_port; }
	void SetUrl(const Buffer& url);
	const Buffer Uri()const { return m_uri; } // ��ȡ URI ·��
private:
	Buffer m_url;
	Buffer m_protocol;
	Buffer m_host;
	Buffer m_uri;
	int m_port;
	std::map<Buffer, Buffer> m_values;
};