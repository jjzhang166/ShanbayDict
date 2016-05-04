#include <QMenu>
#include <QAction>
#include <QDesktopWidget>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QThread>
#include "app/application.h"

std::unique_ptr<Application> DICT::app = std::make_unique<Application>();
std::unique_ptr<Gui> DICT::gui = std::make_unique<Gui>();
std::unique_ptr<ShanbayNet> DICT::shanbayNet = std::make_unique<ShanbayNet>();
std::unique_ptr<Config> DICT::cfg = std::make_unique<Config>();
Dictlogo* DICT::logo;
Application::Application(){
}

void Application::init(){

    DICT::logo = new Dictlogo();
    QObject::connect(DICT::logo,&Dictlogo::Clicked,
                     [&](){
        //qDebug()<<"logo clicked:"<<capture_text;
        DICT::shanbayNet->queryWord(capture_text);
    });

    DICT::gui->init();

    DICT::shanbayNet->connect();

    QObject::connect(DICT::shanbayNet.get(),&ShanbayNet::signalConnectFinished,[&](){
        if(DICT::shanbayNet->state!=ShanbayNet::NetState::login){
            DICT::gui->setLoginWinState("无法连接扇贝网，请稍后重试");
            return;
        }
        DICT::gui->setLoginWinState("已连接扇贝网，请登录!");
    });
    QObject::connect(DICT::shanbayNet.get(),&ShanbayNet::signalShowCaptcha,
                     [&](){
        //qDebug()<<DICT::shanbayNet->captcha.id<<DICT::shanbayNet->captcha.url;
        DICT::gui->showCaptchaImg(DICT::shanbayNet->captcha.url);

    });
    QObject::connect(DICT::gui.get(), &Gui::signalLoginClick,
                     [&](const QString username,QString password,QString captchacode){
        //qDebug()<<"loginClick:"<<username <<password<<captchacode;
        DICT::shanbayNet->login(username,password,captchacode);

    });
    QObject::connect(DICT::gui.get(),&Gui::signalFreshCaptchaImg,
                     [&](){
        //qDebug()<<"require fresh captcha img";
        DICT::shanbayNet->refreshCaptchaImg();
    });
    QObject::connect(DICT::shanbayNet.get(),&ShanbayNet::signalLoginFinished,
                     [&](bool ok,const QString& msg){
        if(ok){
            DICT::gui->loginWin->hide();
            if(!DICT::cfg->isAutohide()) DICT::gui->mainWin->show();
            DICT::cfg->setUsername(DICT::shanbayNet->username);
            if(DICT::cfg->isSavepass()) DICT::cfg->setUserpass(DICT::shanbayNet->password);
            setScreenText();
            showSystrayIcon();
        }else{
            DICT::gui->setLoginWinState(msg);
        }
    });

    QObject::connect(DICT::gui.get(),&Gui::signalBtnqueryClick,
                     [&](const QString word){
        showType = ShowType::main;
        DICT::shanbayNet->queryWord(word);
    });
    QObject::connect(DICT::shanbayNet.get(),&ShanbayNet::signalRetWordinfo,
                     [&](const QString& wordinfo){
        if(showType == ShowType::main){
            DICT::gui->showWord(wordinfo);
        }else{
            DICT::gui->showWordInBalloon(wordinfo);
        }
    });

    QObject::connect(DICT::gui.get(),&Gui::signalBtnaddwordClick,
                     [&](const QString type,const QString id){
        DICT::shanbayNet->addWord(type,id);
    });
    QObject::connect(DICT::shanbayNet.get(),&ShanbayNet::signalAddwordFinished,
                     [&](const QString&data){
        DICT::gui->addWordRet(showType,data);
    });

    QObject::connect(qApp,&QApplication::aboutToQuit,[&](){close();});
}
void Application::close(){
    closeSystrayIcon();
}
void Application::showSystrayIcon(){
    if(trayIcon!=nullptr) return;
    trayIcon = new QSystemTrayIcon(QIcon(":/img/logo.png"),qApp);

    dictMenu = new QMenu("菜单");
    //dictMenu->setLayoutDirection(Qt::LeftToRight);
    showMainWinAction = new QAction(QIcon(":/img/main.ico"),QObject::tr("显示主窗口"),qApp);
    helpAction =new QAction(QIcon(":/img/help.png"),QObject::tr("帮助"),qApp);
    quitAction = new QAction(QIcon(":/img/quit.png"),QObject::tr("退出程序"), qApp);
    cfgAction=new QAction(QIcon(":/img/setup.ico"),QObject::tr("软件设置"),qApp);
    autospeakAction=new QAction(QIcon(":/img/speaker.png"),QObject::tr("自动发音"),qApp);
    getscreenwordAction=new QAction(QIcon(":/img/word.ico"),QObject::tr("屏幕取词"),qApp);
    aboutAction=new QAction(QIcon(":/img/about.png"),QObject::tr("关于"),qApp);

    autospeakAction->setCheckable(true);
    autospeakAction->setChecked(DICT::cfg->isAutospeak());
    getscreenwordAction->setCheckable(true);
    getscreenwordAction->setChecked(DICT::cfg->isGetscreentext());

    dictMenu->addAction(showMainWinAction);
    dictMenu->addAction(helpAction);
    dictMenu->addAction(getscreenwordAction);
    dictMenu->addAction(autospeakAction);
    dictMenu->addAction(cfgAction);
    dictMenu->addAction(aboutAction);
    dictMenu->addSeparator();
    dictMenu->addAction(quitAction);

    trayIcon->setContextMenu(dictMenu);
    QObject::connect(showMainWinAction,&QAction::triggered,[&](){DICT::gui->showMainWin();});
    QObject::connect(autospeakAction,&QAction::toggled,
                     [&](bool checked){
        DICT::cfg->setAutospeak(checked);
        qDebug()<<"autospeak:"<<DICT::cfg->isAutospeak();
    });
    QObject::connect(getscreenwordAction,&QAction::toggled,
                     [&](bool checked){
        DICT::cfg->setScreentext(checked);
        qDebug()<<"getscreenwordAction:"<<checked;
        setScreenText();
    });
    QObject::connect(DICT::cfg.get(),&Config::signalChange,
                     [&](const QString& key, const QVariant& value){
        if(key=="screentext"){
            getscreenwordAction->setChecked(value.toBool());
        }else if(key=="autospeak"){
            autospeakAction->setChecked(value.toBool());
            setScreenText();
        }else if(key=="selectedtext"){
            setScreenText();
        }else if(key=="clipboardtext"){
            setScreenText();
        }
    });
    QObject::connect(quitAction,&QAction::triggered,[&](){qApp->quit();});
    QObject::connect(cfgAction,&QAction::triggered,
                     [&](){
       DICT::gui->showSetupWin();
    });

    //Qt bug:Ubuntu 14.04 Qt 5.6 QSystemTrayIcon doesn't emit activated signal
    QObject::connect(trayIcon,&QSystemTrayIcon::activated,
                     [&](QSystemTrayIcon::ActivationReason reason) {
        switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            qDebug()<<"DoubleClick";
            DICT::gui->showMainWin();
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

}
void Application::captureText(QString text){
    capture_text = text.trimmed();
    if(capture_text.isEmpty()||capture_text.length()>20) return;
    showType = ShowType::balloon;
    if(DICT::cfg->isShowquerylogo()){
        DICT::logo->popup();
        return;
    }
    DICT::shanbayNet->queryWord(capture_text);
}

void Application::setScreenText(){
    if(!DICT::cfg->isGetscreentext()){//关闭屏幕取词
        QObject::disconnect(qApp->clipboard(),0,0,0);
        return;
    }
    if(qApp->clipboard()->supportsSelection()){//windows and os_x not supportSection
        if(DICT::cfg->isGetselectedtext()){
            QObject::connect(qApp->clipboard(),&QClipboard::selectionChanged,
                             [&](){
                qDebug()<<"selectionChanged"<< qApp->clipboard()->mimeData(QClipboard::Selection)->hasText()<<qApp->clipboard()->text(QClipboard::Selection);
                captureText(qApp->clipboard()->text(QClipboard::Selection));
            });

        }else{
            QObject::disconnect(qApp->clipboard(),&QClipboard::selectionChanged,0,0);
        }
    }
    if(DICT::cfg->isGetclipboardtext())   {
        QObject::connect(qApp->clipboard(),&QClipboard::dataChanged,
                         [&](){
            qDebug()<<"dataChanged"<<qApp->clipboard()->mimeData(QClipboard::Clipboard)->hasText()<<qApp->clipboard()->text();
            captureText(qApp->clipboard()->text());
        });
    }else{
        QObject::disconnect(qApp->clipboard(),&QClipboard::dataChanged,0,0);
    }

}
