/**
  ******************************************************************************
  * @file    main.cpp
  * @author  link
  * @version 2.0.0.0
  * @date    2017-02-24
  * @brief   Motor comprehensive tester
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "PageKey.h"
#include "WinHome.h"
#include <QApplication>
#include <QTextCodec>
/**
  * @brief  Initializes the software
  * @param  argc,*argv[]
  * @retval int
  */
int main(int argc, char *argv[])
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
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
#ifdef __arm__
    w.showFullScreen();
#else
    w.show();
    w.resize(1024,768);
#endif
    w.WinInitAll();
    return a.exec();
}
/*********************************END OF FILE**********************************/