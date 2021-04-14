#ifndef _H_C_REFERENCE_OWNER_
#define _H_C_REFERENCE_OWNER_

#include <list>
#include <assert.h>
#include "globals.h"
#include "CReference.h"
#include "CReferenceBase.h"


template<typename T>
class CReferenceOwner final
	: public CReference<T>
{
public:
	CReferenceOwner();
	CReferenceOwner(T* dataPtr);
	~CReferenceOwner();
	CReferenceOwner(const CReferenceOwner& source) = delete;
	CReferenceOwner(CReferenceOwner&& source);
	template<typename U>
	CReferenceOwner(CReferenceOwner<U>&& source);
	CReferenceOwner& operator=(const CReferenceOwner& source) = delete;
	CReferenceOwner& operator=(CReferenceOwner&& source);

	int getRefCount() const;
	CReference<T> getReference() const;
	template<typename U>
	CReference<U> getStaticCastedReference() const;
	template<typename U>
	CReference<U> getDynamicCastedReference() const;
	void deleteReferences();
};


template<typename T>
CReferenceOwner<T>::CReferenceOwner()
	: CReference<T>()
{
	REFERENCE_LOG("[CReferenceOwner]::CReferenceOwner - default constructor");
}


template<typename T>
CReferenceOwner<T>::CReferenceOwner(T* dataPtr)
	: CReference<T>(new std::list<CReferenceBase*>(), static_cast<void*>(dataPtr))
{
	REFERENCE_LOG("[CReferenceOwner]::CReferenceOwner - params constructor");
}


template<typename T>
CReferenceOwner<T>::~CReferenceOwner()
{
	REFERENCE_LOG("[CReferenceOwner]::~CReferenceOwner - destructor");
	this->deleteReferences();
}


template<typename T>
CReferenceOwner<T>::CReferenceOwner(CReferenceOwner&& source)
	: CReference<T>(source.m_referencesList, source.m_dataPtr)
{
	REFERENCE_LOG("[CReferenceOwner]::CReferenceOwner - move constructor");
	source.reset();
}


template<typename T>
template<typename U>
CReferenceOwner<T>::CReferenceOwner(CReferenceOwner<U>&& source)
	: CReference<T>(source.m_referencesList, source.m_dataPtr)
{
	REFERENCE_LOG("[CReferenceOwner]::CReferenceOwner - generalized move constructor");
	// This is only added to cause a compile-time error in case no implicit conversion exists to convert a U* into a T*
	T* ptr = source.get();

	// This, however IS important
	source.reset();
}


template<typename T>
CReferenceOwner<T>& CReferenceOwner<T>::operator=(CReferenceOwner&& source)
{
	REFERENCE_LOG("[CReferenceOwner]::operator= - move assignment");
	if (&source == this)
	{
		return *this;
	}
	this->deleteReferences();

	this->m_referencesList = source.m_referencesList;
	this->m_dataPtr = source.m_dataPtr;
	this->addReference();
	source.reset();

	return *this;
}


template<typename T>
int CReferenceOwner<T>::getRefCount() const
{
	return this->m_referencesList != nullptr ? this->m_referencesList->size() : 0;
}


template<typename T>
CReference<T> CReferenceOwner<T>::getReference() const
{
	return CReference<T>(this->m_referencesList, this->m_dataPtr);
}


template<typename T>
template<typename U>
CReference<U> CReferenceOwner<T>::getStaticCastedReference() const
{
	return this->static_reference_cast<U>();
}


template<typename T>
template<typename U>
CReference<U> CReferenceOwner<T>::getDynamicCastedReference() const
{
	return this->dynamic_reference_cast<U>();
}


template<typename T>
void CReferenceOwner<T>::deleteReferences()
{
	if (this->m_dataPtr == nullptr || this->m_referencesList == nullptr)
	{
		assert(!this->m_dataPtr && !this->m_referencesList);
		return;
	}

	delete static_cast<T*>(this->m_dataPtr);

	for (auto& ref : *(this->m_referencesList))
	{
		if (this != ref)
		{
			ref->m_dataPtr = nullptr;
			ref->m_referencesList = nullptr;
		}
	}
	
	delete this->m_referencesList;
	this->m_referencesList = nullptr;
}


#endif // !_H_C_REFERENCE_OWNER_
