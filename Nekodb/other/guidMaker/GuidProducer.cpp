#include "GuidProducer.h"
#include "../tools/Lock.h"
#include "../tools/tools.h"

CGuidProducer* CGuidProducer::GetInstance()
{
	static CGuidProducer guidProducer;
	return &guidProducer;
}

tstring* CGuidProducer::GetId()
{
	AutoLock lock(m_mutex);
	tstring* pStrId = new tstring(CTools::GuidMaker());
	m_IdList.push_back(pStrId);
	return pStrId;
}

void CGuidProducer::BackId(tstring* pStrId)
{
	AutoLock lock(m_mutex);
	auto iter = std::find(m_IdList.begin(), m_IdList.end(), pStrId);
	if (iter != m_IdList.end()){
		delete *iter;
		m_IdList.erase(iter);
	}
}
