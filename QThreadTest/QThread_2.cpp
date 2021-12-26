#include "QThread_2.h"
#include<qthread.h>
#include<qdebug.h>

MyWork::MyWork(QObject* parent) :QObject(parent) {}

void MyWork::dowork1(int num)
{
	qDebug() << "��ǰ�̶߳���" << QThread::currentThread();
	for (int i = 0; i < num; ++i)
	{
		emit curNumber(i);
		QThread::usleep(1);
	}
	qDebug() << "���̶߳������...";
}

void MyWork::dowork2(int num)
{
	qDebug() << "��ǰ�̶߳���" << QThread::currentThread();
	for (int i = 0; i < num*2; i+=2)
	{
		emit curNumber2(i);
		QThread::usleep(1);
	}
	qDebug() << "���̶߳������...";
	emit workdone1();
}