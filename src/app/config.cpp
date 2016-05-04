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
#include "config.h"
#include <iostream>
#include <QApplication>
Config::Config(QObject *parent) :QObject(parent)
{
    std::cout<<"config load"<<std::endl;
    settings_=new QSettings("shanbay.ini",QSettings::IniFormat);
    load();
}
Config::~Config(){
    std::cout<<"config close"<<settings_->fileName().toStdString()<<std::endl;
    //settings->sync();
    save();
    settings_->deleteLater();
}
void Config::setValue(const QString &key, const QVariant &value) {
    emit signalChange(key,value);
    settings_->setValue(key, value);
}

QVariant Config::value(const QString &key, const QVariant &defaultValue) const {
    return settings_->value(key, defaultValue);
}
void Config::load(){
    username=settings_->value("username","").toString();
    userpass=settings_->value("userpass","").toString();
    savepass=settings_->value("savepass",false).toBool();
    autologin=settings_->value("autologin",false).toBool();
    autospeak=settings_->value("autospeak",true).toBool();
    getscreentext=settings_->value("getscreentext",true).toBool();
#ifdef Q_OS_WIN
    getclipboardtext=settings->value("getclipboardtext",true).toBool();
    getselectedtext=settings->value("getselectedtext",false).toBool();
#else
    getclipboardtext=settings_->value("getclipboardtext",false).toBool();
    getselectedtext=settings_->value("getselectedtext",true).toBool();
#endif
    showquerylogo=settings_->value("showquerylogo",true).toBool();
    autorun=settings_->value("autorun",false).toBool();
    autohide=settings_->value("autohide",true).toBool();
    autoaddword=settings_->value("autoaddword",false).toBool();
}
void Config::save(){
    settings_->setValue("username",username);
    settings_->setValue("userpass",userpass);
    settings_->setValue("savepass",savepass);
    settings_->setValue("autologin",autologin);
    settings_->setValue("autospeak",autospeak);
    settings_->setValue("getscreentext",getscreentext);
    settings_->setValue("getclipboardtext",getclipboardtext);
    settings_->setValue("getselectedtext",getselectedtext);
    settings_->setValue("showquerylogo",showquerylogo);
    settings_->setValue("autorun",autorun);
    settings_->setValue("autohide",autohide);
    settings_->setValue("autoaddword",autoaddword);
    settings_->sync();
}
QString Config::getUsername(){
    return username;
}
void Config::setUsername(QString name){    
    username=name;
}

QString Config::getUserpass(){
    return userpass;
}
void Config::setUserpass(QString text){
    userpass=text;
}

bool Config::isSavepass(){
    return savepass;
}
void Config::setSavepass(bool value){
    savepass=value;
    emit signalChange("savepass",value);
    if(!savepass) userpass="";
}

bool Config::isAutologin(){
    return autologin;
}
void Config::setAutologin(bool value){
    autologin=value;
    emit signalChange("autologin",value);
}

bool Config::isAutohide(){
    return autohide;
}
void Config::setAutohide(bool value){
    autohide=value;
    emit signalChange("autohide",value);
}

bool Config::isAutospeak(){
    return autospeak;
}
void Config::setAutospeak(bool value){
    autospeak=value;
    emit signalChange("autospeak",value);
}

bool Config::isAutorun(){
    return autorun;
}
void Config::setAutorun(bool value){
    autorun=value;
    emit signalChange("autorun",value);
}

bool Config::isGetclipboardtext(){
    return getclipboardtext;
}
void Config::setClipboardtext(bool value){
    getclipboardtext=value;
    emit signalChange("clipboardtext",value);
}
bool Config::isGetselectedtext(){
    return getselectedtext;
}
void Config::setSelectedtext(bool value){
    getselectedtext=value;
    emit signalChange("selectedtext",value);
}

bool Config::isGetscreentext(){
    return getscreentext;
}
void Config::setScreentext(bool value){
    getscreentext=value;
    emit signalChange("screentext",value);
}

bool Config::isShowquerylogo(){
    return showquerylogo;
}
void Config::setShowquerylogo(bool value){
    showquerylogo=value;
    emit signalChange("showquerylogo",value);
}
bool Config::isAutoaddword(){
    return autoaddword;
}
void Config::setAutoaddword(bool value){
    autoaddword=value;
    emit signalChange("autoaddword",value);
}
