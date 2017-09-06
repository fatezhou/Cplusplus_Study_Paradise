#ifndef __GUID_PRODUCER_H__
#define __GUID_PRODUCER_H__

#include "../include/global.h"

class CGuidProducer{
public:
	static CGuidProducer* GetInstance();
	tstring* GetId();
	void BackId(tstring* pStrId);

private:
	std::list<tstring*> m_IdList;
	std::mutex m_mutex;
	CGuidProducer();
};

#endif