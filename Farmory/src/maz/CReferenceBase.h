#ifndef _H_C_REFERENCE_BASE_
#define _H_C_REFERENCE_BASE_

//#define REFERENCE_VERBOSE_LOG
#ifdef REFERENCE_VERBOSE_LOG
#define REFERENCE_LOG(format, ...) MAZ_LOGGER(format, __VA_ARGS__);
#else
#define REFERENCE_LOG(format, ...) (void)0;
#endif


#include <list>

namespace maz
{

class CReferenceBase
{
	template<typename T, bool MEMORY_OWNER>
	friend class CReferenceOwner;
	friend bool operator== (const CReferenceBase& lhs, const CReferenceBase& rhs);

public:
	CReferenceBase();
	virtual ~CReferenceBase() = 0;
	CReferenceBase(const CReferenceBase& source) = delete;
	CReferenceBase& operator=(const CReferenceBase& source) = delete;
	CReferenceBase(CReferenceBase&& source) = delete;
	CReferenceBase& operator=(CReferenceBase&& source) = delete;

	operator bool() const;
	void reset();

protected:
	CReferenceBase(std::list<CReferenceBase*>* referencesList, void* dataPtr);
	void addReference();

	std::list<CReferenceBase*>* m_referencesList = nullptr;
	void* m_dataPtr = nullptr;
};

bool operator==(const CReferenceBase& lhs, const CReferenceBase& rhs);
bool operator!=(const CReferenceBase& lhs, const CReferenceBase& rhs);

} // maz

#endif // !_H_C_REFERENCE_BASE_
