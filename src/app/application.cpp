#include <QMenu>
#include <QAction>
#include <QDesktopWidget>
#include <QApplication>
#include <QClipboard>
#include <QThread>
#include "app/application.h"

std::unique_ptr<Application> DICT::app = std::make_unique<Application>();
std::unique_ptr<Gui> DICT::gui = std::make_unique<Gui>();
std::unique_ptr<ShanbayNet> DICT::shanbayNet = std::make_unique<ShanbayNet>();
std::unique_ptr<Config> DICT::config = std::make_unique<Config>();



Application::Application(){
}

void Application::init(){
    DICT::gui->init();

    DICT::shanbayNet->connect();

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
            DICT::config->setUsername(DICT::shanbayNet->username);
            if(DICT::config->isSavepass()) DICT::config->setUserpass(DICT::shanbayNet->password);
            setScreenText();
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
        DICT::shanbayNet->addWord(type,id);
    });
    QObject::connect(DICT::shanbayNet.get(),&ShanbayNet::signalAddwordFinished,
                     [&](const QString&data){
        DICT::gui->addWordRet(data);
    });


    QObject::connect(qApp,&QApplication::aboutToQuit,[&](){close();});
}
void Application::close(){
    closeSystrayIcon();
}
void Application::showSystrayIcon(){
    if(trayIcon!=nullptr) return;
    trayIcon = new QSystemTrayIcon(QIcon(":/img/logo.png"),qApp);
    dictMenu = new QMenu("菜单",QApplication::desktop());
    //dictMenu->setLayoutDirection(Qt::LeftToRight);
    showMainWinAction = new QAction(QIcon(":/img/main.ico"),QObject::tr("显示主窗口"),qApp);
    helpAction =new QAction(QIcon(":/img/help.png"),QObject::tr("帮助"),qApp);
    quitAction = new QAction(QIcon(":/img/quit.png"),QObject::tr("退出程序"), qApp);
    cfgAction=new QAction(QIcon(":/img/setup.ico"),QObject::tr("软件设置"),qApp);
    autospeakAction=new QAction(QIcon(":/img/sound1.ico"),QObject::tr("自动发音"),qApp);
    getscreenwordAction=new QAction(QIcon(":/img/word.ico"),QObject::tr("屏幕取词"),qApp);
    aboutAction=new QAction(QIcon(":/img/about.png"),QObject::tr("关于"),qApp);

    autospeakAction->setCheckable(true);
    autospeakAction->setChecked(DICT::config->isAutospeak());
    getscreenwordAction->setCheckable(true);
    getscreenwordAction->setChecked(DICT::config->isGetscreentext());

    dictMenu->addAction(showMainWinAction);
    dictMenu->addAction(helpAction);
    dictMenu->addAction(getscreenwordAction);
    dictMenu->addAction(autospeakAction);
    dictMenu->addAction(cfgAction);
    dictMenu->addAction(aboutAction);
    dictMenu->addSeparator();
    dictMenu->addAction(quitAction);

    trayIcon->setContextMenu(dictMenu);

    QObject::connect(autospeakAction,&QAction::toggled,
                     [&](bool checked){
        DICT::config->setAutospeak(checked);
        qDebug()<<"autospeak:"<<DICT::config->isAutospeak();
    });
    QObject::connect(getscreenwordAction,&QAction::toggled,
                     [&](bool checked){
        DICT::config->setScreentext(checked);
        qDebug()<<"getscreenwordAction:"<<checked;
        setScreenText();
    });
    QObject::connect(quitAction,&QAction::triggered,[&](){qApp->quit();});

    //Qt bug:Ubuntu 14.04 Qt 5.6 QSystemTrayIcon doesn't emit activated signal
    QObject::connect(trayIcon,&QSystemTrayIcon::activated,
                     [&](QSystemTrayIcon::ActivationReason reason) {
        switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            qDebug()<<"DoubleClick";
            break;
        case QSystemTrayIcon::MiddleClick:
            qDebug()<<"MiddleClick";
            break;
        default:;
        }
    });
    trayIcon->setToolTip("扇贝词典 2.0");
    trayIcon->show();
}
void Application::closeSystrayIcon(){
    trayIcon->deleteLater();
    dictMenu->deleteLater();
}

void Application::run(){

    init();
    DICT::gui->setLoginWinState("正在连接扇贝网...");
    DICT::gui->loginWin->show();
    showSystrayIcon();
}

void Application::setScreenText(){
    if(!DICT::config->isGetscreentext()){//关闭屏幕取词
#ifdef Q_OS_WIN
        //windows下关闭取词
#endif
        QObject::disconnect(qApp->clipboard(),0,0,0);
        return;
    }

    if(DICT::config->isGetselectedtext()){
#ifdef Q_OS_WIN
        //windows 不支持选中取词
#else
        QObject::connect(qApp->clipboard(),&QClipboard::selectionChanged,
                         [&](){

            qDebug()<<"selectionChanged"<<qApp->clipboard()->text(QClipboard::Selection);
            DICT::shanbayNet->queryWord(qApp->clipboard()->text(QClipboard::Selection));
        });
#endif
    }else{
#ifdef Q_OS_WIN
        //windows下关闭取词
#else
        QObject::disconnect(qApp->clipboard(),&QClipboard::selectionChanged,0,0);
#endif
    }

    if(DICT::config->isGetclipboardtext())   {
        QObject::connect(qApp->clipboard(),&QClipboard::dataChanged,
                         [&](){
            qDebug()<<"dataChanged"<<qApp->clipboard()->text();
            DICT::shanbayNet->queryWord(qApp->clipboard()->text());
        });
    }else{
        QObject::disconnect(qApp->clipboard(),&QClipboard::dataChanged,0,0);
    }

}
