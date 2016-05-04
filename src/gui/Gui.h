#ifndef GUI_H
#define GUI_H
#include <app/application.h>
#include <QObject>
#include <QWindow>
class QQmlApplicationEngine;
class Gui : public QObject
{
    Q_OBJECT
public:
    explicit Gui(QObject *parent = 0);

signals:
    //win send signal to outer
    void signalLoginClick(QString,QString,QString);
    void signalFreshCaptchaImg();

    void signalBtnqueryClick(QString);
    void signalBtnaddwordClick(QString,QString);
    //outer send signal to win
    void signalShowCaptchaImg(const QVariant& url) const;
    void signalSetLoginWinState(const QVariant& str) const;

    void signalShowWord(const QVariant& wordinfo) const;
    void signalAddwordRetMain(const QVariant& data) const;

    void signalAddwordRetBalloon(const QVariant& data) const;

    void signalShowWordInBalloon(const QVariant& wordinfo) const;

public slots:
    void registerClick();
public:
    void init();
    void showCaptchaImg(const QString& url);
    void setLoginWinState(const QString& str);    
    void showMainWin();

    void showWord(const QString& wordinfo);
    void addWordRet(const ShowType type,const QString& data);

    void showWordInBalloon(const QString& wordinfo);

    void showSetupWin();
    QWindow* loginWin;
    QWindow* mainWin;
    QWindow* balloonWin;
    QWindow* setupWin;
private:
    QQmlApplicationEngine *engine;

};
namespace DICT{
    extern std::unique_ptr<Gui> gui;
}
#endif // GUI_H
