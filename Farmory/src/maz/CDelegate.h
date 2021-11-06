#ifndef _H_C_DELEGATE_
#define _H_C_DELEGATE_

#include <maz/globals.h>


namespace maz
{

template< typename R, typename ... P >
class CDelegate
{
public:
    CDelegate()
        : mpObj (nullptr)
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
    static CDelegate Create(const T* apObj)
    {
        CDelegate lDelegate;
        lDelegate.mpObj = const_cast< T* >(apObj);
        lDelegate.mFunc = &HelperFunc< T, Method >;
        return lDelegate;
    }


    template< typename T, R(T:: * Method)(P...) const >
    static CDelegate CreateConst(const T* apObj)
    {
        CDelegate lDelegate;
        lDelegate.mpObj = const_cast< T* >(apObj);
        lDelegate.mFunc = &HelperFunc< T, Method >;
        return lDelegate;
    }


    R operator()(P... aParams)
    {
        return (*mFunc)(mpObj, aParams...);
    }


    bool operator== (const CDelegate& arOther)
    {
        return (mpObj == arOther.mpObj) && (mFunc == arOther.mFunc);
    }


    bool operator!= (const CDelegate& arOther)
    {
        return !(operator==(arOther));
    }


private:
    using THelperFunc = R(*)(void*, P...);
    
    
    template< R(*Function)(P...) >
    static R HelperFunc(void* apObj, P... aParams)
    {
        MAZ_UNUSED_VAR(apObj);
        return (*Function)(aParams...);
    }


    template< typename T, R(T::* Method)(P...) >
    static R HelperFunc(void* apObj, P... aParams)
    {
        MAZ_ASSERT(apObj != nullptr, "nullptr passed as apObj!");
        return ((static_cast< T* >(apObj))->*Method)(aParams...);
    }


    template< typename T, R(T::* Method)(P...) const >
    static R HelperFunc(void* apObj, P... aParams)
    {
        return ((static_cast< T* >(apObj))->*Method)(aParams...);
    }


    void* mpObj;
    THelperFunc mFunc;
};

} // maz

#endif // !_H_C_DELEGATE_
