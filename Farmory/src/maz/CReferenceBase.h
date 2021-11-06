#ifndef _H_C_REFERENCE_BASE_
#define _H_C_REFERENCE_BASE_

#include <maz/globals.h>
#include <list>

//#define REFERENCE_VERBOSE_LOG
#ifdef REFERENCE_VERBOSE_LOG
#define REFERENCE_LOG(format, ...) MAZ_LOGGER(format, __VA_ARGS__);
#else
#define REFERENCE_LOG(format, ...) (void)0;
#endif


namespace maz
{

class CReferenceBase
{
	template<typename T, bool MEMORY_OWNER>
	friend class CReferenceMaster;
	friend bool operator== (const CReferenceBase& arLhs, const CReferenceBase& arRhs);

public:
	CReferenceBase();
	virtual ~CReferenceBase() = 0;
	CReferenceBase(const CReferenceBase& arSource) = delete;
	CReferenceBase& operator=(const CReferenceBase& arSource) = delete;
	CReferenceBase(CReferenceBase&& arrSource) = delete;
	CReferenceBase& operator=(CReferenceBase&& arrSource) = delete;

	operator bool() const;
	void reset();

protected:
	CReferenceBase(std::list<CReferenceBase*>* apReferencesList, void* apDataPtr);
	void addReference();

	std::list<CReferenceBase*>* mpReferencesList = nullptr;
	void* mpData = nullptr;
};

bool operator==(const CReferenceBase& arLhs, const CReferenceBase& arRhs);
bool operator!=(const CReferenceBase& arLhs, const CReferenceBase& arRhs);

} // maz

#endif // !_H_C_REFERENCE_BASE_
