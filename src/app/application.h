#ifndef APPLICATION_H
#define APPLICATION_H
#include <QDebug>
#include <memory>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <src/net/shanbaynet.h>
#include "gui/Gui.h"
#include "config.h"
#include "dictlogo.h"
enum class ShowType{balloon,main};
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
    extern std::unique_ptr<Gui> gui;
    extern std::unique_ptr<ShanbayNet> shanbayNet;
    extern std::unique_ptr<Config> config;
    extern Dictlogo* logo;
}
#endif // APPLICATION_H
