/****************************************************************************
**
** Copyright (C) 2013 Jeffrey Lee.
** All rights reserved.
** Contact: vipjeffreylee@gmail.com
**
** This file is part of the ShanbayDict.
**
** $BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
**
**
** $END_LICENSE$
**
****************************************************************************/
#ifndef SHANBAYNET_H
#define SHANBAYNET_H
#include <QObject>

class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

class ShanbayNet : public QObject
{
    Q_OBJECT
public:
    explicit ShanbayNet(QObject *parent = 0);
    ~ShanbayNet();
    enum class HttpAction{NoAction,GetSessionidAction,RefreshCaptchaImg,LoginAction,QueryWordAction,AddWordAction,AddExampleAction,QueryWordExamplesAction};
    enum class NetState{connect,login,ok};
signals:
    void signalLoginFinished(bool ok,const QString& msg);
    void signalQueryFinished();
    void signalAddwordFinished(const QString&data);
    void signalQueryExampleFinished();
    void signalAddExampleFinished(const QString &msg);
    void signalConnectFinished();
    void signalShowCaptcha();
    void signalRetWordinfo(const QString &wordinfo);
public slots:
    void httpfinished(QNetworkReply*);
public://属性
    NetState state=NetState::connect;
    QNetworkAccessManager * http;
    QString queryword;
    QString nickname;
    QString username;
    QString password;
    class Captcha{
    public:
        QString id;//captcha code id
        QString code;//captcha code
        QString url;//captcha picture url
    public:
        void clear(){id=code=url="";}
        bool isneed(){return !id.isEmpty();}
        void parseHtml(const QString&);
    };
    Captcha captcha{"","",""};
public://方法
    void connect();
    void login(const QString &u,const QString &p,const QString &c);
    void refreshCaptchaImg();
    void queryWord(const QString &word);
    void addWord(const QString &type,const QString&id);
    void queryExamples(QString learningid);
    void addExample(QString learning_id,QString sentence,QString translation);
private:
    void loginShanbay();
    HttpAction httpAction;
    QString getCookie(const QString &name);
    void getSessionid();
    bool sessionidOk(){
        return !sessionid.isEmpty();
    }
    QString sessionid;
};

#endif // SHANBAYNET_H
