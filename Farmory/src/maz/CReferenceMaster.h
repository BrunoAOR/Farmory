#ifndef _H_C_REFERENCE_OWNER_
#define _H_C_REFERENCE_OWNER_

#include <list>
#include "globals.h"
#include "CReference.h"
#include "CReferenceBase.h"


namespace maz
{

template<typename T, bool MEMORY_OWNER = true>
class CReferenceMaster final
	: public CReference<T>
{
public:
	CReferenceMaster();
	CReferenceMaster(T* apData);
	~CReferenceMaster();
	CReferenceMaster(const CReferenceMaster& arSource) = delete;
	CReferenceMaster(CReferenceMaster&& arrSource);
	template<typename U>
	CReferenceMaster(CReferenceMaster<U>&& arrSource);
	CReferenceMaster& operator=(const CReferenceMaster& arSource) = delete;
	CReferenceMaster& operator=(CReferenceMaster&& arrSource);

	int GetRefCount() const;
	CReference<T> GetReference() const;
	template<typename U>
	CReference<U> GetStaticCastedReference() const;
	template<typename U>
	CReference<U> GetDynamicCastedReference() const;
	void DeleteReferences();
};


template<typename T, bool MEMORY_OWNER>
CReferenceMaster<T, MEMORY_OWNER>::CReferenceMaster()
	: CReference<T>()
{
	REFERENCE_LOG("Default constructor");
}


template<typename T, bool MEMORY_OWNER>
CReferenceMaster<T, MEMORY_OWNER>::CReferenceMaster(T* apData)
	: CReference<T>(MAZ_NEW(std::list<CReferenceBase*>), static_cast<void*>(apData))
{
	REFERENCE_LOG("Params constructor");
}


template<typename T, bool MEMORY_OWNER>
CReferenceMaster<T, MEMORY_OWNER>::~CReferenceMaster()
{
	REFERENCE_LOG("Destructor");
	this->DeleteReferences();
}


template<typename T, bool MEMORY_OWNER>
CReferenceMaster<T, MEMORY_OWNER>::CReferenceMaster(CReferenceMaster&& arrSource)
	: CReference<T>(arrSource.mpReferencesList, arrSource.mpData)
{
	REFERENCE_LOG("Move constructor");
	arrSource.reset();
}


template<typename T, bool MEMORY_OWNER>
template<typename U>
CReferenceMaster<T, MEMORY_OWNER>::CReferenceMaster(CReferenceMaster<U>&& arrSource)
	: CReference<T>(arrSource.mpReferencesList, arrSource.mpData)
{
	REFERENCE_LOG("Generalized move constructor");
	// This is only added to cause a compile-time error in case no implicit conversion exists to convert a U* into a T*
	T* pData = arrSource.get();
	MAZ_UNUSED_VAR(pData);

	// This, however IS important
	arrSource.reset();
}


template<typename T, bool MEMORY_OWNER>
CReferenceMaster<T, MEMORY_OWNER>& CReferenceMaster<T, MEMORY_OWNER>::operator=(CReferenceMaster&& arrSource)
{
	REFERENCE_LOG("Move assignment");
	if (&arrSource == this)
	{
		return *this;
	}
	this->DeleteReferences();

	this->mpReferencesList = arrSource.mpReferencesList;
	this->mpData = arrSource.mpData;
	this->addReference();
	arrSource.reset();

	return *this;
}


template<typename T, bool MEMORY_OWNER>
int CReferenceMaster<T, MEMORY_OWNER>::GetRefCount() const
{
	return this->mpReferencesList != nullptr ? this->mpReferencesList->size() : 0;
}


template<typename T, bool MEMORY_OWNER>
CReference<T> CReferenceMaster<T, MEMORY_OWNER>::GetReference() const
{
	return CReference<T>(this->mpReferencesList, this->mpData);
}


template<typename T, bool MEMORY_OWNER>
template<typename U>
CReference<U> CReferenceMaster<T, MEMORY_OWNER>::GetStaticCastedReference() const
{
	return this->static_reference_cast<U>();
}


template<typename T, bool MEMORY_OWNER>
template<typename U>
CReference<U> CReferenceMaster<T, MEMORY_OWNER>::GetDynamicCastedReference() const
{
	return this->dynamic_reference_cast<U>();
}


template<typename T, bool MEMORY_OWNER>
void CReferenceMaster<T, MEMORY_OWNER>::DeleteReferences()
{
	if (this->mpData == nullptr || this->mpReferencesList == nullptr)
	{
		MAZ_ASSERT(!this->mpData && !this->mpReferencesList, "If mpData is nullptr, then mpREferencesList MUST also be nullptr (and vice versa)!");
		return;
	}

	if constexpr (MEMORY_OWNER)
	{
		delete static_cast<T*>(this->mpData);
	}

	for (CReferenceBase* lRef : *(this->mpReferencesList))
	{
		if (this != lRef)
		{
			lRef->mpData = nullptr;
			lRef->mpReferencesList = nullptr;
		}
	}
	
	delete this->mpReferencesList;
	this->mpReferencesList = nullptr;
	this->mpData = nullptr;
}


template<typename T>
using CReferenceOwner = CReferenceMaster<T, true>;
template<typename T>
using CReferenceHolder = CReferenceMaster<T, false>;

} // maz

#endif // !_H_C_REFERENCE_OWNER_
