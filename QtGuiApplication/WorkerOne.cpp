#include "WorkerOne.h"
#include "log4cplus/log4cplus.h"

#define GET_LOGGER(x) log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(x))

WorkerOne::WorkerOne(QObject *parent)
	: QObject(parent)
{
}

WorkerOne::~WorkerOne()
{
}

void WorkerOne::initAll(QString portName)
{
}

void WorkerOne::recTrigger(QString value)
{
}
