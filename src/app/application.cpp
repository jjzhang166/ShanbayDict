#include <QMenu>
#include <QAction>
#include <QDesktopWidget>
#include <QApplication>
#include "app/application.h"

std::unique_ptr<Application> DICT::app = std::make_unique<Application>();
std::unique_ptr<Gui> DICT::gui = std::make_unique<Gui>();
std::unique_ptr<ShanbayNet> DICT::shanbayNet = std::make_unique<ShanbayNet>();

QMenu *dictMenu;
QAction *getscreenwordAction;
QAction *cfgAction;
QAction *autospeakAction;
QAction *quitAction;
QAction *aboutAction;
QAction *helpAction;

Application::Application(){}

void Application::init(){
    DICT::gui->init();

    DICT::shanbayNet->connect();
    trayIcon = new QSystemTrayIcon(qApp);
    QObject::connect(DICT::shanbayNet.get(),&ShanbayNet::signalConnectFinished,[&](){
        if(DICT::shanbayNet->state!=ShanbayNet::NetState::login){
            DICT::gui->setLoginWinState("无法连接扇贝网，请稍后重试");
            return;
        }
        DICT::gui->setLoginWinState("扇贝网已经连接，请登录");

    });
    QObject::connect(DICT::shanbayNet.get(),&ShanbayNet::signalShowCaptcha,
                     [&](){
         qDebug()<<DICT::shanbayNet->captcha.id<<DICT::shanbayNet->captcha.url;
         DICT::gui->showCaptchaImg(DICT::shanbayNet->captcha.url);

    });
    QObject::connect(DICT::gui.get(), &Gui::signalLoginClick,
                      [&](const QString username,QString password,QString captchacode){
        qDebug()<<"loginClick:"<<username <<password<<captchacode;
        DICT::shanbayNet->login(username,password,captchacode);

    });
    QObject::connect(DICT::gui.get(),&Gui::signalFreshCaptchaImg,
                      [&](){
        qDebug()<<"require fresh captcha img";
        DICT::shanbayNet->refreshCaptchaImg();
    });
    QObject::connect(DICT::shanbayNet.get(),&ShanbayNet::signalLoginFinished,
                     [&](bool ok,const QString& msg){
        if(ok){
            DICT::gui->loginWin->hide();
            DICT::gui->mainWin->show();
            //DICT::shanbayNet->queryWord("good");
        }else{
            DICT::gui->setLoginWinState(msg);
        }
    });

    QObject::connect(DICT::gui.get(),&Gui::signalBtnqueryClick,
                     [&](const QString word){
       DICT::shanbayNet->queryWord(word);
    });
    QObject::connect(DICT::shanbayNet.get(),&ShanbayNet::signalRetWordinfo,
                     [&](const QString& wordinfo){
        DICT::gui->showWord(wordinfo);
    });

    QObject::connect(DICT::gui.get(),&Gui::signalBtnaddwordClick,
                     [&](const QString type,const QString id){
        qDebug()<<"add word:"<<type<<id;
        DICT::shanbayNet->addWord(type,id);
    });
    QObject::connect(DICT::shanbayNet.get(),&ShanbayNet::signalAddwordFinished,
                     [&](const QString&data){
        qDebug()<<"addword finished:"<<data;
        DICT::gui->addWordRet(data);

    });


    QObject::connect(qApp,&QApplication::aboutToQuit,
                     [&](){
        qDebug()<< "application closing";
        dictMenu->deleteLater();
    });
}


void Application::run(){

    init();
    DICT::gui->setLoginWinState("正在连接扇贝网...");
    DICT::gui->loginWin->show();

    ////////////////////////////////////////
    dictMenu = new QMenu("菜单",QApplication::desktop());
    //dictMenu  = new QMenu("菜单");
    //dictMenu->setLayoutDirection(Qt::LeftToRight);
    helpAction =new QAction(QObject::tr("帮助"),qApp);
    helpAction->setIcon(QIcon(":/img/help.png"));
    quitAction = new QAction(QObject::tr("退出程序"), qApp);
    cfgAction=new QAction(QObject::tr("软件设置"),qApp);
    autospeakAction=new QAction(QObject::tr("自动发音"),qApp);
    getscreenwordAction=new QAction(QObject::tr("屏幕取词"),qApp);
    aboutAction=new QAction(QObject::tr("关于"),qApp);
    aboutAction->setIcon(QIcon(":/img/about.png"));
    autospeakAction->setCheckable(true);
    getscreenwordAction->setCheckable(true);
    quitAction->setIcon(QIcon(":/img/quit.png"));

    dictMenu->addAction(helpAction);
    dictMenu ->addAction(getscreenwordAction);
    dictMenu ->addAction(autospeakAction);
    dictMenu ->addAction(cfgAction);
    dictMenu->addAction(aboutAction);
    dictMenu ->addSeparator();
    dictMenu ->addAction(quitAction);

    QIcon icon = QIcon(":/img/logo.png");
    trayIcon->setContextMenu(dictMenu);
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("title");
    trayIcon->show();


    ///////////////////////////////////////
}
