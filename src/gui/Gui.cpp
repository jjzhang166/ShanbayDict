#include "Gui.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <app/application.h>
#include <QDesktopServices>


Gui::Gui(QObject *parent) : QObject(parent)
{

}
void Gui::init(){

    engine = new QQmlApplicationEngine();
    QQmlContext *context = engine->rootContext();
    context->setContextProperty("cfg", DICT::cfg.get());

    engine->load(QUrl(QStringLiteral("qrc:/src/gui/Login.qml")));
    loginWin = qobject_cast<QWindow*>(engine->rootObjects().at(0));


    //loginWin->setFlags(Qt::FramelessWindowHint);
    //qDebug()<<engine.rootObjects().size()<<loginWin->objectName();
    QObject::connect(loginWin,SIGNAL(signalRegisterClick()),
                     this,SLOT(registerClick()));

    QObject::connect(loginWin, SIGNAL(signalLoginClick(QString,QString,QString)),
                     this, SIGNAL(signalLoginClick(QString,QString,QString)));
    QObject::connect(loginWin,SIGNAL(signalFreshCaptchaImg()),
                     this,SIGNAL(signalFreshCaptchaImg()));

    QObject::connect(this,SIGNAL(signalShowCaptchaImg(QVariant)),
                     loginWin,SLOT(showCaptchaImg(QVariant)));
    QObject::connect(this,SIGNAL(signalSetLoginWinState(QVariant)),
                     loginWin,SLOT(setState(QVariant)));

    engine->load(QUrl(QStringLiteral("qrc:/src/gui/main.qml")));
    mainWin = qobject_cast<QWindow*>(engine->rootObjects().at(1));
    QObject::connect(mainWin,SIGNAL(signalBtnqueryClick(QString)),
                     this,SIGNAL(signalBtnqueryClick(QString)));
    QObject::connect(this,SIGNAL(signalShowWord(QVariant)),
                     mainWin,SLOT(showWord(QVariant)));

    QObject::connect(mainWin,SIGNAL(signalBtnaddwordClick(QString,QString)),
                     this,SIGNAL(signalBtnaddwordClick(QString,QString)));
    QObject::connect(this,SIGNAL(signalAddwordRet(QVariant)),
                     mainWin,SLOT(addWordRet(QVariant)));

    engine->load(QUrl(QStringLiteral("qrc:/src/gui/Balloon.qml")));
    balloonWin = qobject_cast<QWindow*>(engine->rootObjects().at(2));
    QObject::connect(this,SIGNAL(signalShowWordInBalloon(QVariant)),
                     balloonWin,SLOT(showWord(QVariant)));

}
//loginWin
void Gui::showCaptchaImg(const QString &url){
    emit signalShowCaptchaImg(url);
}
void Gui::setLoginWinState(const QString &str){
    emit signalSetLoginWinState(str);
}
//mainWin
void Gui::showMainWin(){
    //mainWin->hide();
    mainWin->showNormal();
}

void Gui::showWord(const QString &wordinfo){
    emit signalShowWord(wordinfo);
}
void Gui::addWordRet(const QString &data){
    emit signalAddwordRet(data);
}

void Gui::showWordInBalloon(const QString &wordinfo){
    balloonWin->setPosition(QCursor::pos()+QPoint(10,15));
    balloonWin->show();
    emit signalShowWordInBalloon(wordinfo);
}
void Gui::registerClick(){
    QDesktopServices::openUrl(QUrl("http://www.shanbay.com/referral/ref/9e54b69ab8/"));
}