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

    CDelegateBase(R(*apFct)(P)) : mpFct(apFct) { ; }
    virtual ~CDelegateBase() { MAZ_LOGGER("Destroy CDelegateBase"); }

    virtual R Invoke(P aParam) { return mpFct(aParam); }

protected:
    CDelegateBase(void* apObj) : mpObj(apObj) { ; }

protected:
    union
    {
        void* mpObj;
        TFct  mpFct;
    };
};


template< typename T, typename R, typename P >
class CMethodDelegate
    : public CDelegateBase< R, P >
{
    using TMFct = R(T::*)(P);

public:
    CMethodDelegate(T* apObj, R(T::* aMFct)(P)) : CDelegateBase< R, P >(apObj), mpMFct(aMFct) { ; }
    virtual ~CMethodDelegate() { MAZ_LOGGER("Destroy CMethodDelegate"); }

    virtual R Invoke(P aParam)
    {
        return ((static_cast< T* >(CDelegateBase< R, P >::mpObj))->*mpMFct)(aParam);
    }

private:
    TMFct mpMFct;
};

} // impl


template< typename R, typename P >
class CDelegateHolder
{
public:
    CDelegateHolder();

    // Copy and move contructors
    CDelegateHolder(const CDelegateHolder& arOther) = delete;
    CDelegateHolder(CDelegateHolder&& arrOther);

    // Copy and move assignments
    CDelegateHolder& operator=(const CDelegateHolder& arOther) = delete;
    CDelegateHolder& operator=(CDelegateHolder&& arrOther);

    CDelegateHolder(R(*apFct)(P));

    template< typename T >
    CDelegateHolder(T* aObj, R(T::* apFct)(P));

    ~CDelegateHolder();

    R operator()(P aParam);

private:
    struct TPlaceholder {};
    unsigned char mDelegateBuffer[sizeof(impl::CMethodDelegate< TPlaceholder, R, P >)];
};


template< typename T, typename R, typename P >
CDelegateHolder< R, P > BindMethod(T* apObj, R(T::* apFct)(P))
{
    return CDelegateHolder< R, P >(apObj, apFct);
}


template< typename R, typename P >
CDelegateHolder< R, P > BindFunction(R(* apFct)(P))
{
    return CDelegateHolder< R, P >(apFct);
}

} // maz

#include "CDelegateOld.inl"

#endif // !_H_C_DELEGATE_OLD_
