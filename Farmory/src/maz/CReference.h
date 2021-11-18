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
	CReference<U> static_reference_cast() const;
#ifdef MAZ_RTTI
	template<typename U>
	bool is_castable_as() const;
	template<typename U>
	CReference<U> dynamic_reference_cast() const;
#endif // MAZ_RTTI

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
	// Ensure type T is base class of U. If U is base class of T, then static_reference_cast should be used to obtain a CReference.
	static_assert(std::is_base_of<T, U>());
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
CReference<U> CReference<T>::static_reference_cast() const
{
	// If static_casting, either U should be a base of T (safe cast) or T should be a base of U (and the user better be sure of that!)
	static_assert(std::is_base_of<U, T>() || std::is_base_of<T, U>());
	return CReference<U>(this->mpReferencesList, this->mpData);
}


#ifdef MAZ_RTTI
template<typename T>
template<typename U>
bool CReference<T>::is_castable_as() const
{
	return (dynamic_cast<U*>(static_cast<T*>(this->mpData)));
}


template<typename T>
template<typename U>
CReference<U> CReference<T>::dynamic_reference_cast() const
{
	if (is_castable_as<U>())
	{
		return static_reference_cast<U>();
	}
	else
	{
		return CReference<U>();
	}
}
#endif // MAZ_RTTI


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
