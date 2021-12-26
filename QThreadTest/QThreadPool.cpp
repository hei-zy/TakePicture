#include"QThreadPool.h"
#include<qdebug.h>
#include<qthread.h>


MyWork1::MyWork1(QObject* parent) :QObject(parent), QRunnable()
{
	setAutoDelete(false);
}

void MyWork1::run()
{
	qDebug() << "��ǰ�̶߳���ĵ�ַ��" << QThread::currentThread();

	int num = 0;
	while (1)
	{
		emit curNumber(num++);
		if (num == 1000)
			break;
		QThread::usleep(1);
	}
	qDebug() << "run() ִ����ϣ����߳��˳�...";
}


MyWork2::MyWork2(QObject* parent) :QObject(parent), QRunnable()
{
	setAutoDelete(false);
}

void MyWork2::run()
{
	qDebug() << "��ǰ�̶߳���ĵ�ַ��" << QThread::currentThread();

	int num = 0;
	while (1)
	{
		emit curNumber(num++);
		if (num == 1000)
			break;
		QThread::usleep(1);
	}
	qDebug() << "run() ִ����ϣ����߳��˳�...";
}