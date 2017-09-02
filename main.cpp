/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       电机综合测试仪
*******************************************************************************/
#include "PageKey.h"
#include "WinHome.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[]) {
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

    WinHome w;
    w.showMaximized();

    return a.exec();
}
/*********************************END OF FILE**********************************/
