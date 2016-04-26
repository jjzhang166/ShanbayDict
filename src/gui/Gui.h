#ifndef GUI_H
#define GUI_H

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
    void signalClosing();
    void signalLoginClick(QString,QString,QString);
    void signalFreshCaptchaImg();

    void signalBtnqueryClick(QString);
    void signalBtnaddwordClick(QString,QString);
    //outer send signal to win
    void signalShowCaptchaImg(const QVariant& url) const;
    void signalSetLoginWinState(const QVariant& str) const;

    void signalShowWord(const QVariant& wordinfo) const;
    void signalAddwordRet(const QVariant& data) const;

public slots:
public:
    void init();
    void showCaptchaImg(const QString& url);
    void setLoginWinState(const QString& str);

    void showWord(const QString& wordinfo);
    void addWordRet(const QString& data);
    QWindow* loginWin;
    QWindow* mainWin;
private:
    QQmlApplicationEngine *engine;
};

#endif // GUI_H
