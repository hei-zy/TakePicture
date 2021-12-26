#include"QThreadPool.h"
#include<qdebug.h>
#include<qthread.h>


MyWork1::MyWork1(QObject* parent) :QObject(parent), QRunnable()
{
	setAutoDelete(false);
}

void MyWork1::run()
{
	qDebug() << "当前线程对象的地址：" << QThread::currentThread();

	int num = 0;
	while (1)
	{
		emit curNumber(num++);
		if (num == 1000)
			break;
		QThread::usleep(1);
	}
	qDebug() << "run() 执行完毕，子线程退出...";
}


MyWork2::MyWork2(QObject* parent) :QObject(parent), QRunnable()
{
	setAutoDelete(false);
}

void MyWork2::run()
{
	qDebug() << "当前线程对象的地址：" << QThread::currentThread();

	int num = 0;
	while (1)
	{
		emit curNumber(num++);
		if (num == 1000)
			break;
		QThread::usleep(1);
	}
	qDebug() << "run() 执行完毕，子线程退出...";
}