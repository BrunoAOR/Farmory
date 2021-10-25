#include "CReferenceBase.h"

#include <assert.h>


namespace maz
{

bool operator==(const CReferenceBase& lhs, const CReferenceBase& rhs)
{
	return (lhs.m_referencesList && rhs.m_referencesList && lhs.m_referencesList == rhs.m_referencesList);
}


bool operator!=(const CReferenceBase& lhs, const CReferenceBase& rhs)
{
	return !operator==(lhs, rhs);
}


CReferenceBase::CReferenceBase()
{
}


CReferenceBase::CReferenceBase(std::list<CReferenceBase*>* referencesList, void* dataPtr)
	: m_referencesList(referencesList), m_dataPtr(dataPtr)
{
	addReference();
}


CReferenceBase::~CReferenceBase()
{
	reset();
}


CReferenceBase::operator bool() const
{
	return m_dataPtr != nullptr;
}


void CReferenceBase::addReference()
{
	if (m_referencesList != nullptr)
	{
		m_referencesList->push_back(this);
	}
}


void CReferenceBase::reset()
{
	if (m_referencesList != nullptr)
	{
		std::list<CReferenceBase*>::iterator it = std::find(m_referencesList->begin(), m_referencesList->end(), this);
		assert(it != m_referencesList->end());
		m_referencesList->erase(it);
		if (m_referencesList->size() == 0)
		{
			delete m_referencesList;
		}
		m_referencesList = nullptr;
		m_dataPtr = nullptr;
	}
}

} // maz
