#include <new>

namespace maz
{

template< typename R, typename P >
inline CDelegateHolder< R, P >::CDelegateHolder()
{
    MAZ_LOGGER("Construct CDelegateHolder");
    memset(mDelegateBuffer, 0, sizeof(mDelegateBuffer));
}


template< typename R, typename P >
inline CDelegateHolder< R, P >::~CDelegateHolder()
{
    MAZ_LOGGER("Destroy CDelegateHolder");
    reinterpret_cast<impl::CDelegateBase< R, P >*>(mDelegateBuffer)->~CDelegateBase();
}


template< typename R, typename P >
inline CDelegateHolder< R, P >::CDelegateHolder(R(*aFct)(P))
{
    MAZ_PLACEMENT_NEW(mDelegateBuffer, (impl::CDelegateBase< R, P >), aFct);
}


template< typename R, typename P >
template< typename T >
inline CDelegateHolder< R, P >::CDelegateHolder(T* aObj, R(T::* aFct)(P))
{
    MAZ_PLACEMENT_NEW(mDelegateBuffer, (impl::CMethodDelegate< T, R, P >), aObj, aFct);
}


template< typename R, typename P >
inline CDelegateHolder< R, P >::CDelegateHolder(CDelegateHolder&& aOther)
{
    memmove_s(mDelegateBuffer, sizeof(mDelegateBuffer), aOther.mDelegateBuffer, sizeof(aOther.mDelegateBuffer));
}


template< typename R, typename P >
inline CDelegateHolder< R, P>& CDelegateHolder< R, P >::operator=(CDelegateHolder&& aOther)
{
    memmove_s(mDelegateBuffer, sizeof(mDelegateBuffer), aOther.mDelegateBuffer, sizeof(aOther.mDelegateBuffer));
}


template< typename R, typename P >
inline R CDelegateHolder< R, P >::operator()(P aParam)
{
    return reinterpret_cast<impl::CDelegateBase< R, P >*>(mDelegateBuffer)->Invoke(aParam);
}


} // maz
