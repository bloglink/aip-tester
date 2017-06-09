#include "desktop.h"
#include <QApplication>
#include <QTextCodec>
#include "PageKey.h"

int main(int argc, char *argv[])
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif

    QApplication a(argc, argv);

#ifdef __arm__
    PageKey::Instance()->hide();
#endif

    Desktop w;
    w.show();

    return a.exec();
}
