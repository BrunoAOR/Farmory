#ifndef _H_C_DELEGATE_OLD_
#define _H_C_DELEGATE_OLD_

#include <maz/globals.h>

namespace maz
{
namespace impl
{
template< typename R, typename P >
class CDelegateBase
{
    using TFct = R(*)(P);
public:

    CDelegateBase(R(*aFct)(P)) : mFct(aFct) { ; }
    virtual ~CDelegateBase() { MAZ_LOGGER("Destroy CDelegateBase"); }

    virtual R Invoke(P aParam) { return mFct(aParam); }

protected:
    CDelegateBase(void* aObj) : mObj(aObj) { ; }

protected:
    union
    {
        void* mObj;
        TFct  mFct;
    };
};


template< typename T, typename R, typename P >
class CMethodDelegate
    : public CDelegateBase< R, P >
{
    using TMFct = R(T::*)(P);

public:
    CMethodDelegate(T* aObj, R(T::* aMFct)(P)) : CDelegateBase< R, P >(aObj), mMFct(aMFct) { ; }
    virtual ~CMethodDelegate() { MAZ_LOGGER("Destroy CMethodDelegate"); }

    virtual R Invoke(P aParam)
    {
        return ((static_cast< T* >(CDelegateBase< R, P >::mObj))->*mMFct)(aParam);
    }

private:
    TMFct mMFct;
};

} // impl

template< typename R, typename P >
class CDelegateHolder
{
public:
    CDelegateHolder();

    // Copy and move contructors
    CDelegateHolder(const CDelegateHolder& aOther) = delete;
    CDelegateHolder(CDelegateHolder&& aOther);

    // Copy and move assignments
    CDelegateHolder& operator=(const CDelegateHolder& aOther) = delete;
    CDelegateHolder& operator=(CDelegateHolder&& aOther);

    CDelegateHolder(R(*aFct)(P));

    template< typename T >
    CDelegateHolder(T* aObj, R(T::* aFct)(P));

    ~CDelegateHolder();

    R operator()(P aParam);

private:
    struct TPlaceholder {};
    unsigned char mDelegateBuffer[sizeof(impl::CMethodDelegate< TPlaceholder, R, P >)];
};


template< typename T, typename R, typename P >
CDelegateHolder< R, P > BindMethod(T* aObj, R(T::* aFct)(P))
{
    return CDelegateHolder< R, P >(aObj, aFct);
}


template< typename R, typename P >
CDelegateHolder< R, P > BindFunction(R(* aFct)(P))
{
    return CDelegateHolder< R, P >(aFct);
}

} // maz

#include "CDelegateOld.inl"

#endif // !_H_C_DELEGATE_OLD_
