#ifndef APPLICATION_H
#define APPLICATION_H
#include <QDebug>
#include <memory>
#include <QSystemTrayIcon>
#include <src/net/shanbaynet.h>
#include "gui/Gui.h"

class Application
{
public:
    explicit Application();
public:
    void init();
    void run();
private:
    QSystemTrayIcon* trayIcon;
};

namespace DICT{
    extern std::unique_ptr<Application> app;
    extern std::unique_ptr<Gui> gui;
    extern std::unique_ptr<ShanbayNet> shanbayNet;
}
#endif // APPLICATION_H
