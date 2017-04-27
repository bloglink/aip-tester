/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       MessageBox定位
*******************************************************************************/
#ifndef CUSTOM_WINDOW_H
#define CUSTOM_WINDOW_H

#include <QtGui/QDialog>


class CustomWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CustomWindow(QWidget *parent = 0);
    ~CustomWindow();
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    bool        mMoveing;
    QPoint      mMovePosition;
};

#endif // CUSTOM_WINDOW_H
