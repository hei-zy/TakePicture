#pragma once

#include<qobject.h>
#include<qrunnable.h>


class MyWork1 :public QObject,public QRunnable
{
	Q_OBJECT
public:
	MyWork1(QObject* parent = nullptr);
public:
	void run()override;
signals:
	void curNumber(int num);
};

class MyWork2 :public QObject, public QRunnable
{
	Q_OBJECT
public:
	MyWork2(QObject* parent = nullptr);
public:
	void run()override;
signals:
	void curNumber(int num);
};