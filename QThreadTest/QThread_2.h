#pragma once

#include<qobject.h>

class MyWork :public QObject
{
	Q_OBJECT
public:
	MyWork(QObject* parent = nullptr);

	void dowork1(int num);
	void dowork2(int num);
signals:
	void workdone1();
	void curNumber(int i);
	void curNumber2(int i);
};