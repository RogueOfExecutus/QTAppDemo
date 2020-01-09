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

void WorkerOne::initAll()
{
}

void WorkerOne::recTrigger(std::string value)
{
}

void WorkerOne::stopRunning()
{
}
