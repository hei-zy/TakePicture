#include "QThread_2.h"
#include<qthread.h>
#include<qdebug.h>

MyWork::MyWork(QObject* parent) :QObject(parent) {}

void MyWork::dowork1(int num)
{
	qDebug() << "当前线程对象：" << QThread::currentThread();
	for (int i = 0; i < num; ++i)
	{
		emit curNumber(i);
		QThread::usleep(1);
	}
	qDebug() << "子线程对象结束...";
}

void MyWork::dowork2(int num)
{
	qDebug() << "当前线程对象：" << QThread::currentThread();
	for (int i = 0; i < num*2; i+=2)
	{
		emit curNumber2(i);
		QThread::usleep(1);
	}
	qDebug() << "子线程对象结束...";
	emit workdone1();
}