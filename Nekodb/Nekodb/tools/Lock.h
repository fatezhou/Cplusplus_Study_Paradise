#ifndef __LOCK_H__
#define __LOCK_H__

#include "../include/global.h"

class AutoLock{
public:
	AutoLock(std::mutex& _mutex) : m_mutex(_mutex){
		m_mutex.lock();
	};

	~AutoLock(){
		m_mutex.unlock();
	};
private:
	std::mutex& m_mutex;
};

#endif