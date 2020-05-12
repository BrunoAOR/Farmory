#ifndef _H_C_DELEGATE_
#define _H_C_DELEGATE_

#include <maz/globals.h>

namespace maz
{
struct TTest
{
    TTest(int a) : mMult(a) { ; }

    int myFunc(float aF)
    {
        return static_cast<int>(aF)* mMult;
    }

    int myFunc(float aF) const
    {
        return static_cast<int>(aF)* mMult * 1000;
    }

    int myConstFunc(float aF) const
    {
        return static_cast<int>(aF) * 2 * mMult;
    }

    static int myStaticFunc(float aF)
    {
        return static_cast<int>(aF) * 100;
    }

    int mMult;
};

template< typename R, typename ... P >
class CDelegate
{
public:
    CDelegate()
        : mObj (nullptr)
        , mFunc(nullptr)
    { ; }
    
    template< R(*Func)(P...) >
    static CDelegate Create()
    {
        CDelegate lDelegate;
        lDelegate.mFunc = &HelperFunc< Func >;
        return lDelegate;
    }

    template< typename T, R(T:: * Method)(P...) >
    static CDelegate Create(const T* aObj)
    {
        CDelegate lDelegate;
        lDelegate.mObj = const_cast< T* >(aObj);
        lDelegate.mFunc = &HelperFunc< T, Method >;
        return lDelegate;
    }

    template< typename T, R(T:: * Method)(P...) const >
    static CDelegate CreateConst(const T* aObj)
    {
        CDelegate lDelegate;
        lDelegate.mObj = const_cast< T* >(aObj);
        lDelegate.mFunc = &HelperFunc< T, Method >;
        return lDelegate;
    }

    R operator()(P... aParams)
    {
        return (*mFunc)(mObj, aParams...);
    }

    bool operator== (const CDelegate& aOther)
    {
        return (mObj == aOther.mObj) && (mFunc == aOther.mFunc);
    }

    bool operator!= (const CDelegate& aOther)
    {
        return !(operator==(aOther));
    }

private:
    using THelperFunc = R(*)(void*, P...);
    
    void*       mObj;
    THelperFunc mFunc;

    template< R(*Function)(P...) >
    static R HelperFunc(void* aObj, P... aParams)
    {
        MAZ_UNUSED_VAR(aObj);
        return (*Function)(aParams...);
    }

    template< typename T, R(T::* Method)(P...) >
    static R HelperFunc(void* aObj, P... aParams)
    {
        return ((static_cast< T* >(aObj))->*Method)(aParams...);
    }

    template< typename T, R(T::* Method)(P...) const >
    static R HelperFunc(void* aObj, P... aParams)
    {
        return ((static_cast< T* >(aObj))->*Method)(aParams...);
    }

};

} // maz

#endif // !_H_C_DELEGATE_
