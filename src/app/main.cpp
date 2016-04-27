#include <QApplication>
#include "app/application.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
#ifdef Q_OS_LINUX
    QFont font;
    font.setFamily("WenQuanYi Micro Hei");
    qApp->setFont(font);
#endif
    DICT::app->run();
    return app.exec();
 }
