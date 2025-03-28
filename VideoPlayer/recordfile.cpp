﻿#include "recordfile.h"
#include <QFile>
#include <QDebug>

RecordFile::RecordFile(const QString& path)
{
    QFile file(path);
    m_path = path;
    qDebug() << m_path;
    do
    {
        if(!file.open(QIODevice::ReadOnly))
        {
            break;
        }
        QByteArray data = file.readAll();
        if(data.size() <= 0)
        {
            break;
        }
        data = tool.rsaDecode(data);
        //qDebug() << __FILE__ << "(" << __LINE__ << "):" << data;
        int i = 0;
        for(; i < data.size(); i++)
        {
            if((int)data[i] >= (int)0x7F || (int)data[i] < (int)0x0A)
            {
                data.resize(i);
                break;
            }
        }
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):" << data;
        //qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):" << data.size();
        QJsonParseError json_error;
        QJsonDocument doucment = QJsonDocument::fromJson(data, &json_error);
        if (json_error.error == QJsonParseError::NoError)
        {
            if (doucment.isObject())
            {
                m_config = doucment.object();
                //qDebug() << __FILE__ << "(" << __LINE__ << "):" << m_config;
                return;//成功了，直接返回
            }
        }
        else
        {
            qDebug().nospace() << __FILE__ << "(" << __LINE__ << "):" << json_error.errorString() << json_error.error;
        }
    }
    while(false);
    //读取失败，则设置默认值
    file.close();
    QJsonValue value = QJsonValue();
    m_config.insert("user", value);
    m_config.insert("password", value);
    m_config.insert("auto", false);//自动登录
    m_config.insert("remember", false);//记住密码
    m_config.insert("date", "2021-04-01 10:10:10");//
    //qDebug() << __FILE__ << "(" << __LINE__ << "):" << m_config;
    return;
}

RecordFile::~RecordFile()
{
    //qDebug() << __FILE__ << "(" << __LINE__ << "):";
    save();
}

QJsonObject& RecordFile::config()
{
    return m_config;
}

bool RecordFile::save()
{
    QFile file(m_path);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate) == false)
    {
        //qDebug() << __FILE__ << "(" << __LINE__ << "):";
        return false;
    }
    //qDebug() << __FILE__ << "(" << __LINE__ << "):";
    QJsonDocument doucment = QJsonDocument(m_config);
    //qDebug() << __FILE__ << "(" << __LINE__ << "):";
    file.write(tool.rsaEncode(doucment.toJson()));
    //qDebug() << __FILE__ << "(" << __LINE__ << "):";
    file.close();
    //qDebug() << __FILE__ << "(" << __LINE__ << "):" << m_config;
    return true;
}
