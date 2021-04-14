#ifndef _H_C_REFERENCE_OWNER_
#define _H_C_REFERENCE_OWNER_

#include <list>
#include <assert.h>
#include "globals.h"
#include "CReference.h"
#include "CReferenceBase.h"


template<typename T, bool MEMORY_OWNER = true>
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

	int GetRefCount() const;
	CReference<T> GetReference() const;
	template<typename U>
	CReference<U> GetStaticCastedReference() const;
	template<typename U>
	CReference<U> GetDynamicCastedReference() const;
	void DeleteReferences();
};


template<typename T, bool MEMORY_OWNER>
CReferenceOwner<T, MEMORY_OWNER>::CReferenceOwner()
	: CReference<T>()
{
	REFERENCE_LOG("[CReferenceOwner]::CReferenceOwner - default constructor");
}


template<typename T, bool MEMORY_OWNER>
CReferenceOwner<T, MEMORY_OWNER>::CReferenceOwner(T* dataPtr)
	: CReference<T>(new std::list<CReferenceBase*>(), static_cast<void*>(dataPtr))
{
	REFERENCE_LOG("[CReferenceOwner]::CReferenceOwner - params constructor");
}


template<typename T, bool MEMORY_OWNER>
CReferenceOwner<T, MEMORY_OWNER>::~CReferenceOwner()
{
	REFERENCE_LOG("[CReferenceOwner]::~CReferenceOwner - destructor");
	this->DeleteReferences();
}


template<typename T, bool MEMORY_OWNER>
CReferenceOwner<T, MEMORY_OWNER>::CReferenceOwner(CReferenceOwner&& source)
	: CReference<T>(source.m_referencesList, source.m_dataPtr)
{
	REFERENCE_LOG("[CReferenceOwner]::CReferenceOwner - move constructor");
	source.reset();
}


template<typename T, bool MEMORY_OWNER>
template<typename U>
CReferenceOwner<T, MEMORY_OWNER>::CReferenceOwner(CReferenceOwner<U>&& source)
	: CReference<T>(source.m_referencesList, source.m_dataPtr)
{
	REFERENCE_LOG("[CReferenceOwner]::CReferenceOwner - generalized move constructor");
	// This is only added to cause a compile-time error in case no implicit conversion exists to convert a U* into a T*
	T* ptr = source.get();
	MAZ_UNUSED_VAR(ptr);

	// This, however IS important
	source.reset();
}


template<typename T, bool MEMORY_OWNER>
CReferenceOwner<T, MEMORY_OWNER>& CReferenceOwner<T, MEMORY_OWNER>::operator=(CReferenceOwner&& source)
{
	REFERENCE_LOG("[CReferenceOwner]::operator= - move assignment");
	if (&source == this)
	{
		return *this;
	}
	this->DeleteReferences();

	this->m_referencesList = source.m_referencesList;
	this->m_dataPtr = source.m_dataPtr;
	this->addReference();
	source.reset();

	return *this;
}


template<typename T, bool MEMORY_OWNER>
int CReferenceOwner<T, MEMORY_OWNER>::GetRefCount() const
{
	return this->m_referencesList != nullptr ? this->m_referencesList->size() : 0;
}


template<typename T, bool MEMORY_OWNER>
CReference<T> CReferenceOwner<T, MEMORY_OWNER>::GetReference() const
{
	return CReference<T>(this->m_referencesList, this->m_dataPtr);
}


template<typename T, bool MEMORY_OWNER>
template<typename U>
CReference<U> CReferenceOwner<T, MEMORY_OWNER>::GetStaticCastedReference() const
{
	return this->static_reference_cast<U>();
}


template<typename T, bool MEMORY_OWNER>
template<typename U>
CReference<U> CReferenceOwner<T, MEMORY_OWNER>::GetDynamicCastedReference() const
{
	return this->dynamic_reference_cast<U>();
}


template<typename T, bool MEMORY_OWNER>
void CReferenceOwner<T, MEMORY_OWNER>::DeleteReferences()
{
	if (this->m_dataPtr == nullptr || this->m_referencesList == nullptr)
	{
		assert(!this->m_dataPtr && !this->m_referencesList);
		return;
	}

	if constexpr (MEMORY_OWNER)
	{
		delete static_cast<T*>(this->m_dataPtr);
	}

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
	this->m_dataPtr = nullptr;
}


#endif // !_H_C_REFERENCE_OWNER_
