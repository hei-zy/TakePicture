#include "QThreadTest.h"
#include"qdebug.h"


//#include"QThread_1.h"
//QThreadTest::QThreadTest(QWidget *parent)
//    : QMainWindow(parent)
//{
//    ui.setupUi(this);
//    
//    qDebug() << "主线程对象地址：" << QThread::currentThread();
//    MyThread* subThread = new MyThread;
//    connect(ui.startBtn, &QPushButton::clicked, this, [=]()
//        {
//            subThread->start();
//        });
//    connect(subThread, &MyThread::curNumber, this, [=](int num)
//        {
//            ui.label->setText(QString::number(num));
//        });
//
//
//    /*线程资源的回收
//    * 1.使用对象树
//    * 2.设置窗口销毁事件
//    */
//    connect(this, &QMainWindow::destroyed, this, [=]()
//        {
//            subThread->quit();
//            subThread->wait();
//            subThread->deleteLater();
//        });
//}


#include"QThread_2.h"
#include<qthread.h>
QThreadTest::QThreadTest(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    qDebug() << "主线程对象地址：" << QThread::currentThread();
    
    QThread* th1 = new QThread;
    QThread* th2 = new QThread;
    
    MyWork* work1 = new MyWork;
    MyWork* work2 = new MyWork;

    work1->moveToThread(th1);
    work2->moveToThread(th2);

    connect(ui.startBtn, &QPushButton::clicked, this, [=]()
        {
            emit sendnum(1000);
            th1->start();
            th2->start();
        });

    connect(this, &QThreadTest::sendnum, work1, &MyWork::dowork1);
    connect(work1, &MyWork::curNumber, this, [=](int num)
        {
            ui.label->setText(QString::number(num));
        });

    connect(this, &QThreadTest::sendnum, work2, &MyWork::dowork2);
    connect(work2, &MyWork::curNumber2, this, [=](int num)
        {
            ui.label_2->setText(QString::number(num));
        });


    /*线程资源的回收
    * 1.使用对象树
    * 2.设置窗口销毁事件
    */
    connect(this, &QMainWindow::destroyed, this, [=]()
        {
            th1->quit();
            th1->wait();
            th1->deleteLater();

            th2->quit();
            th2->wait();
            th2->deleteLater();

            work1->deleteLater();
            work2->deleteLater();
        });
}

//#include"QThreadPool.h"
//#include<qthreadpool.h>
//QThreadTest::QThreadTest(QWidget *parent)
//    : QMainWindow(parent)
//{
//    ui.setupUi(this);
//    
//    qDebug() << "主线程对象地址：" << QThread::currentThread();
//    QThreadPool::globalInstance()->setMaxThreadCount(4);
//    MyWork1* task1 = new MyWork1;
//    MyWork2* task2 = new MyWork2;
//
//    connect(ui.startBtn, &QPushButton::clicked, this, [=]()
//        {
//            QThreadPool::globalInstance()->start(task1);
//            QThreadPool::globalInstance()->start(task2);
//        });
//    connect(task1, &MyWork1::curNumber, this, [=](int i)
//        {
//            ui.label->setText(QString::number(i));
//        });
//    connect(task2, &MyWork2::curNumber, this, [=](int i)
//        {
//            ui.label_2->setText(QString::number(i));
//        });
//
//
//    /*线程资源的回收
//    * 1.使用对象树
//    * 2.设置窗口销毁事件
//    * 3.设置线程池自动回收
//    */
//    connect(this, &QMainWindow::destroyed, this, [=]()
//        {
//            task1->deleteLater();
//        });
//}