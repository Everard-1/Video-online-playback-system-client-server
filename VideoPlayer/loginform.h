﻿#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QDesktopServices>

#include "infoform.h"
#include "recordfile.h"


namespace Ui
{
    class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT
    friend class testVideoPlayer;
public:
    explicit LoginForm(QWidget* parent = nullptr);
    ~LoginForm();
    virtual bool eventFilter(QObject* watched, QEvent* event);
    virtual void timerEvent(QTimerEvent* event);
protected slots:
    void on_logoButton_released();//登录按钮
    void on_remberPwd_stateChanged(int state);//记住密码
    void slots_autoLoginCheck_stateChange(int state);//自动登录
    void slots_login_request_finshed(QNetworkReply* reply);//接收服务器应答
signals:
    void login(const QString& nick, const QByteArray& head);
protected:
    virtual void mouseMoveEvent(QMouseEvent* event);//移动窗口
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
private:
    bool check_login(const QString& user, const QString& pwd);//检测登录状态
    void load_config();//加载配置文件
private:
    Ui::LoginForm* ui;
    QNetworkAccessManager* net;
    InfoForm info;
    RecordFile* record;
    int auto_login_id;
    QPoint position;//鼠标位置
};

#endif // LOGINFORM_H
