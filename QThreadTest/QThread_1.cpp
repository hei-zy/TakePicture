#include "QThread_1.h"
#include<qdebug.h>

MyThread::MyThread(QObject* parent) :QThread(parent) {}


void MyThread::run()
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
