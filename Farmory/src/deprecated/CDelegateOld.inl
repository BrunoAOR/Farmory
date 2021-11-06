#include <new>


namespace maz
{

template< typename R, typename P >
inline CDelegateHolder< R, P >::CDelegateHolder()
{
    MAZ_LOGGER("Called");
    memset(mDelegateBuffer, 0, sizeof(mDelegateBuffer));
}


template< typename R, typename P >
inline CDelegateHolder< R, P >::~CDelegateHolder()
{
    MAZ_LOGGER("Called");
    reinterpret_cast<impl::CDelegateBase< R, P >*>(mDelegateBuffer)->~CDelegateBase();
}


template< typename R, typename P >
inline CDelegateHolder< R, P >::CDelegateHolder(R(*apFct)(P))
{
    MAZ_PLACEMENT_NEW(mDelegateBuffer, (impl::CDelegateBase< R, P >), apFct);
}


template< typename R, typename P >
template< typename T >
inline CDelegateHolder< R, P >::CDelegateHolder(T* apObj, R(T::* apFct)(P))
{
    MAZ_PLACEMENT_NEW(mDelegateBuffer, (impl::CMethodDelegate< T, R, P >), apObj, apFct);
}


template< typename R, typename P >
inline CDelegateHolder< R, P >::CDelegateHolder(CDelegateHolder&& arrOther)
{
    memmove_s(mDelegateBuffer, sizeof(mDelegateBuffer), arrOther.mDelegateBuffer, sizeof(arrOther.mDelegateBuffer));
}


template< typename R, typename P >
inline CDelegateHolder< R, P>& CDelegateHolder< R, P >::operator=(CDelegateHolder&& arrOther)
{
    memmove_s(mDelegateBuffer, sizeof(mDelegateBuffer), arrOther.mDelegateBuffer, sizeof(arrOther.mDelegateBuffer));
}


template< typename R, typename P >
inline R CDelegateHolder< R, P >::operator()(P aParam)
{
    return reinterpret_cast<impl::CDelegateBase< R, P >*>(mDelegateBuffer)->Invoke(aParam);
}

} // maz
