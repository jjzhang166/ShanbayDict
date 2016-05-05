/****************************************************************************
**
** Copyright (C) 2016 lieefu Way.
** All rights reserved.
** Contact: lieefu@gmail.com
**
** This file is part of the ShanbayDict.
**
** $BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 3 as published by the Free Software Foundation and
** appearing in the file LICENSE included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 3 requirements will be met:
** http://www.gnu.org/licenses/lgpl.html.
**
** $END_LICENSE$
**                                                    2016/5/5
****************************************************************************/
#ifndef APPLICATION_H
#define APPLICATION_H
enum class ShowType{balloon,main};
#include <QDebug>
#include <memory>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <src/net/shanbaynet.h>
#include "gui/Gui.h"
#include "config.h"
#include "dictlogo.h"
class Application
{
public:
    explicit Application();
public:
    void init();
    void run();
    void close();
    void showSystrayIcon();
    void closeSystrayIcon();
    void setScreenText();//根据config设置开启或者关闭屏幕取词
    void captureText(QString text);//捕获到文本信息()

private:
    QString capture_text;
    ShowType showType;
    QSystemTrayIcon* trayIcon=nullptr;
    QMenu *dictMenu;
    QAction *showMainWinAction;
    QAction *getscreenwordAction;
    QAction *cfgAction;
    QAction *autospeakAction;
    QAction *quitAction;
    QAction *aboutAction;
    QAction *helpAction;
};

namespace DICT{
    extern std::unique_ptr<Application> app;
    extern std::unique_ptr<ShanbayNet> shanbayNet;
    extern std::unique_ptr<Config> cfg;
    extern Dictlogo* logo;
}
#endif // APPLICATION_H
