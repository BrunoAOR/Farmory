#ifndef _H_C_REFERENCE_
#define _H_C_REFERENCE_

#define MAZ_LOG_VERBOSE

#include "CReferenceBase.h"
#include "globals.h"


namespace maz
{

template<typename T>
class CReference
	: public CReferenceBase
{
	template<typename U>
	friend class CReference;
	template<typename U, bool MEMORY_OWNER>
	friend class CReferenceOwner;

public:
	CReference();
	virtual ~CReference();
	CReference(const CReference& source);
	template<typename U>
	CReference(const CReference<U>& source);
	CReference& operator=(const CReference& source);

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
	CReference(std::list<CReferenceBase*>* referencesList, void* dataPtr);
};


template<typename T>
CReference<T>::CReference()
	: CReferenceBase()
{
	REFERENCE_LOG("[CReference]::CReference - default constructor");
}


template<typename T>
CReference<T>::CReference(std::list<CReferenceBase*>* referencesList, void* dataPtr)
	: CReferenceBase(referencesList, dataPtr)
{
	REFERENCE_LOG("[CReference]::CReference - params constructor");
}


template<typename T>
CReference<T>::~CReference()
{
	REFERENCE_LOG("[CReference]::~CReference - destructor");
	// CReferenceBase will call removeReference upon destruction.
}


template<typename T>
CReference<T>::CReference(const CReference& source)
	: CReferenceBase(source.m_referencesList, source.m_dataPtr)
{
	REFERENCE_LOG("[CReference]::CReference - copy constructor");
}


template<typename T>
template<typename U>
CReference<T>::CReference(const CReference<U>& source)
	: CReferenceBase(source.m_referencesList, source.m_dataPtr)
{
	REFERENCE_LOG("[CReference]::CReference - generalized copy constructor");
	// This is only added to cause a compile-time error in case no implicit conversion exists to convert a U* into a T*
	const T* ptr = static_cast<U*>(source.m_dataPtr);
}


template<typename T>
CReference<T>& CReference<T>::operator=(const CReference& source)
{
	REFERENCE_LOG("[CReference]::operator= - copy assignment");
	if (&source == this)
	{
		return *this;
	}
	reset();
	m_referencesList = source.m_referencesList;
	m_dataPtr = source.m_dataPtr;
	addReference();
	return *this;
}


template<typename T>
template<typename U>
bool CReference<T>::is_castable_as() const
{
	return (dynamic_cast<U*>(static_cast<T*>(this->m_dataPtr)));
}


template<typename T>
template<typename U>
CReference<U> CReference<T>::static_reference_cast() const
{
	return CReference<U>(this->m_referencesList, this->m_dataPtr);
}


template<typename T>
template<typename U>
CReference<U> CReference<T>::dynamic_reference_cast() const
{
	if (dynamic_cast<U*>(static_cast<T*>(this->m_dataPtr)))
	{
		return CReference<U>(this->m_referencesList, this->m_dataPtr);
	}
	else
	{
		return CReference<U>();
	}
}


template<typename T>
T* CReference<T>::get()
{
	return static_cast<T*>(m_dataPtr);
}

template<typename T>
const T& CReference<T>::operator*() const
{
	return *(static_cast<T*>(m_dataPtr));
}


template<typename T>
T& CReference<T>::operator*()
{
	return *(static_cast<T*>(m_dataPtr));
}


template<typename T>
const T* CReference<T>::operator->() const
{
	return static_cast<T*>(m_dataPtr);
}


template<typename T>
T* CReference<T>::operator->()
{
	return static_cast<T*>(m_dataPtr);
}

} // maz

#endif // !_H_C_REFERENCE_
