#ifndef _H_C_REFERENCE_
#define _H_C_REFERENCE_

#include "CReferenceBase.h"


namespace maz
{

template<typename T>
class CReference
	: public CReferenceBase
{
	template<typename U>
	friend class CReference;
	template<typename U, bool MEMORY_OWNER>
	friend class CReferenceMaster;

public:
	CReference();
	virtual ~CReference();
	CReference(const CReference& arSource);
	template<typename U>
	CReference(const CReference<U>& arSource);
	CReference& operator=(const CReference& arSource);

	template<typename U>
	bool is_castable_as() const;
	template<typename U>
	CReference<U> static_reference_cast() const;
	template<typename U>
	CReference<U> dynamic_reference_cast() const;

	T* get();
	const T& operator*() const;
	T& operator*();
	const T* operator->() const;
	T* operator->();

private:
	CReference(std::list<CReferenceBase*>* apReferencesList, void* apDataPtr);
};


template<typename T>
CReference<T>::CReference()
	: CReferenceBase()
{
	REFERENCE_LOG("Default constructor");
}


template<typename T>
CReference<T>::CReference(std::list<CReferenceBase*>* apReferencesList, void* apDataPtr)
	: CReferenceBase(apReferencesList, apDataPtr)
{
	REFERENCE_LOG("Params constructor");
}


template<typename T>
CReference<T>::~CReference()
{
	REFERENCE_LOG("Destructor");
	// CReferenceBase will call removeReference upon destruction.
}


template<typename T>
CReference<T>::CReference(const CReference& arSource)
	: CReferenceBase(arSource.mpReferencesList, arSource.mpData)
{
	REFERENCE_LOG("Copy constructor");
}


template<typename T>
template<typename U>
CReference<T>::CReference(const CReference<U>& arSource)
	: CReferenceBase(arSource.mpReferencesList, arSource.mpData)
{
	REFERENCE_LOG("Generalized copy constructor");
	// This is only added to cause a compile-time error in case no implicit conversion exists to convert a U* into a T*
	const T* ptr = static_cast<U*>(arSource.mpData);
}


template<typename T>
CReference<T>& CReference<T>::operator=(const CReference& arSource)
{
	REFERENCE_LOG("Copy assignment");
	if (&arSource == this)
	{
		return *this;
	}
	reset();
	mpReferencesList = arSource.mpReferencesList;
	mpData = arSource.mpData;
	addReference();
	return *this;
}


template<typename T>
template<typename U>
bool CReference<T>::is_castable_as() const
{
	return (dynamic_cast<U*>(static_cast<T*>(this->mpData)));
}


template<typename T>
template<typename U>
CReference<U> CReference<T>::static_reference_cast() const
{
	return CReference<U>(this->mpReferencesList, this->mpData);
}


template<typename T>
template<typename U>
CReference<U> CReference<T>::dynamic_reference_cast() const
{
	if (dynamic_cast<U*>(static_cast<T*>(this->mpData)))
	{
		return CReference<U>(this->mpReferencesList, this->mpData);
	}
	else
	{
		return CReference<U>();
	}
}


template<typename T>
T* CReference<T>::get()
{
	return static_cast<T*>(mpData);
}

template<typename T>
const T& CReference<T>::operator*() const
{
	return *(static_cast<T*>(mpData));
}


template<typename T>
T& CReference<T>::operator*()
{
	return *(static_cast<T*>(mpData));
}


template<typename T>
const T* CReference<T>::operator->() const
{
	return static_cast<T*>(mpData);
}


template<typename T>
T* CReference<T>::operator->()
{
	return static_cast<T*>(mpData);
}

} // maz

#endif // !_H_C_REFERENCE_
