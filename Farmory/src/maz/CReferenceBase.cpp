#include "CReferenceBase.h"


namespace maz
{

bool operator==(const CReferenceBase& arLhs, const CReferenceBase& arRhs)
{
	return (arLhs.mpReferencesList && arRhs.mpReferencesList && (arLhs.mpReferencesList == arRhs.mpReferencesList));
}


bool operator!=(const CReferenceBase& arLhs, const CReferenceBase& arRhs)
{
	return !operator==(arLhs, arRhs);
}


CReferenceBase::CReferenceBase()
{
}


CReferenceBase::CReferenceBase(std::list<CReferenceBase*>* apReferencesList, void* apData)
	: mpReferencesList(apReferencesList), mpData(apData)
{
	addReference();
}


CReferenceBase::~CReferenceBase()
{
	reset();
}


CReferenceBase::operator bool() const
{
	return mpData != nullptr;
}


void CReferenceBase::addReference()
{
	if (mpReferencesList != nullptr)
	{
		mpReferencesList->push_back(this);
	}
}


void CReferenceBase::reset()
{
	if (mpReferencesList != nullptr)
	{
		std::list<CReferenceBase*>::iterator it = std::find(mpReferencesList->begin(), mpReferencesList->end(), this);
		MAZ_ASSERT(it != mpReferencesList->end(), "Couldn't find the current CReferenceBase in the mpReferenceList!");
		mpReferencesList->erase(it);
		if (mpReferencesList->size() == 0)
		{
			delete mpReferencesList;
		}
		mpReferencesList = nullptr;
		mpData = nullptr;
	}
}

} // maz
