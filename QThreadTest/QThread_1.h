#pragma once

#include<qthread.h>
#include<qobject.h>


class MyThread :public QThread
{
	Q_OBJECT
public:
	MyThread(QObject* parent = nullptr);
protected:
	void run()override;
signals:
	void curNumber(int num);
};