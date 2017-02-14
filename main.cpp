#include "MainPage.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
    QApplication a(argc, argv);

    MainPage w;
//    w.showFullScreen();
    w.show();
    w.resize(1024,768);

    return a.exec();
}
