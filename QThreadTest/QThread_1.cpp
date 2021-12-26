#include "QThread_1.h"
#include<qdebug.h>

MyThread::MyThread(QObject* parent) :QThread(parent) {}


void MyThread::run()
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
