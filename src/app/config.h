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
#ifndef CONFIG_H
#define CONFIG_H
#include <QSettings>
#include <QObject>
class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = 0);
    ~Config();

signals:

public slots:

private:
    QString username,userpass;
    bool savepass, autologin, autospeak, autoaddword,autorun,autohide,
    getscreentext,
    getclipboardtext,
    getselectedtext,
    showquerylogo;
    QSettings *settings_;
    void load();
    void save();
public:
    Q_INVOKABLE void setValue(const QString & key, const QVariant & value);
    Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;


    Q_INVOKABLE QString getUsername();
    Q_INVOKABLE void setUsername(QString name);

    Q_INVOKABLE QString getUserpass();
    Q_INVOKABLE void setUserpass(QString text);

    Q_INVOKABLE bool isSavepass();
    Q_INVOKABLE void setSavepass(bool value);

    Q_INVOKABLE bool isAutologin();
    Q_INVOKABLE void setAutologin(bool value);

    Q_INVOKABLE bool isAutospeak();
    Q_INVOKABLE void setAutospeak(bool value);

    Q_INVOKABLE bool isGetscreentext();
    Q_INVOKABLE void setScreentext(bool value);

    Q_INVOKABLE bool isGetclipboardtext();
    Q_INVOKABLE void setClipboardtext(bool value);

    Q_INVOKABLE bool isGetselectedtext();
    Q_INVOKABLE void setSelectedtext(bool value);

    Q_INVOKABLE bool isShowquerylogo();
    Q_INVOKABLE void setShowquerylogo(bool value);

    Q_INVOKABLE bool isAutorun();
    Q_INVOKABLE void setAutorun(bool value);

    Q_INVOKABLE bool isAutohide();
    Q_INVOKABLE void setAutohide(bool value);

    Q_INVOKABLE bool isAutoaddword();
    Q_INVOKABLE void setAutoaddword(bool value);
};

#endif // CONFIG_H
