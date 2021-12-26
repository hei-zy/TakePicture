#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QThreadTest.h"

class QThreadTest : public QMainWindow
{
    Q_OBJECT

public:
    QThreadTest(QWidget *parent = Q_NULLPTR);
signals:
    void sendnum(int i);
private:
    Ui::QThreadTestClass ui;
};
