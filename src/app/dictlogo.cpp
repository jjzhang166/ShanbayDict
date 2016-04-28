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
#include "dictlogo.h"


Dictlogo::Dictlogo(QWidget *parent) :
    QLabel(parent)
{
#ifdef Q_OS_WIN
    setWindowFlags(Qt::Popup);
#else
    setWindowFlags(Qt::FramelessWindowHint|Qt::ToolTip);
#endif
    setGeometry(0,0,32,32);
    setPixmap(QPixmap(":/img/logo.png").scaled(QSize(32,32), Qt::IgnoreAspectRatio));
    timer=new QTimer();
    connect(timer,SIGNAL(timeout()), this, SLOT(autohide()));
}
void Dictlogo::popup(){
    hide();
    timer->stop();
    setWindowOpacity(qreal(0.9));
    move(QCursor::pos()-QPoint(36,46));
    show();
    timercount=0.9;
    timer->start(500);
}
void Dictlogo::autohide(){
    if(!this->rect().contains(this->mapFromGlobal(QCursor::pos()))){
        this->setWindowOpacity(timercount);
        if(timercount<0){
            timer->stop();
            this->hide();
        }
        timercount-=0.1;
        mousein=false;
    }else{
        if(!mousein){
            mousein=true;
            timercount=0.9;
            this->setWindowOpacity(qreal(0.9));
        }
    }
}
