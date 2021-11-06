#ifndef _H_C_FIXED_SIZE_STRING_
#define _H_C_FIXED_SIZE_STRING_

#include <maz/globals.h>


namespace maz
{

template<uint8 SIZE>
class CFixedSizeString
{
public:
    CFixedSizeString();
    CFixedSizeString(const char* apSource);
    template<uint8 OTHER_SIZE>
    CFixedSizeString(const CFixedSizeString<OTHER_SIZE>& arSource);
    ~CFixedSizeString();

    uint8 GetSize() const;
    uint8 GetCapacity() const;

    char* Get();
    const char* c_str() const;

private:
    char mBuffer[SIZE];
};


template<uint8 SIZE>
inline CFixedSizeString<SIZE>::CFixedSizeString()
{
    mBuffer[0] = '\0';
}


template<uint8 SIZE>
CFixedSizeString<SIZE>::CFixedSizeString(const char* apSource)
{
    MAZ_ASSERT(apSource != nullptr, "Called with a nullptr source!");
    const uint32 lSourceSize = static_cast<uint32>(strlen(apSource));
    MAZ_ASSERT(lSourceSize < SIZE - 1, "The size of the source string (%u) may not be larger than the size of the CFixedSizeString (%hhu) (minus the null terminator). The source string will be clamped!", lSourceSize, SIZE);
    const uint8 lCharsToCopy = static_cast<uint8>(Min<uint32>(lSourceSize, SIZE - 1));
    strncpy_s(mBuffer, apSource, lCharsToCopy);
    mBuffer[lCharsToCopy] = '\0';
}


template<uint8 SIZE>
template<uint8 OTHER_SIZE>
inline CFixedSizeString<SIZE>::CFixedSizeString(const CFixedSizeString<OTHER_SIZE>& arSource)
{
    const uint8 lSourceSize = arSource.GetSize();
    MAZ_ASSERT(lSourceSize < SIZE - 1, "The size of the source string (%hhu) may not be larger than the size of the CFixedSizeString (%hhu) (minus the null terminator). The source string will be clamped!", lSourceSize, SIZE);
    const uint8 lCharsToCopy = Min<uint32>(lSourceSize, SIZE - 1);
    strncpy(mBuffer, arSource.c_str(), lCharsToCopy);
    mBuffer[lCharsToCopy] = '\0';
}


template<uint8 SIZE>
CFixedSizeString<SIZE>::~CFixedSizeString()
{}


template<uint8 SIZE>
uint8 CFixedSizeString<SIZE>::GetSize() const
{
    return strlen(mBuffer);
}


template<uint8 SIZE>
uint8 CFixedSizeString<SIZE>::GetCapacity() const
{
    return SIZE;
}


template<uint8 SIZE>
char* CFixedSizeString<SIZE>::Get()
{
    return mBuffer;
}


template<uint8 SIZE>
const char* CFixedSizeString<SIZE>::c_str() const
{
    return mBuffer;
}


using CFixedString16 = CFixedSizeString<16>;
using CFixedString32 = CFixedSizeString<32>;
using CFixedString64 = CFixedSizeString<64>;

} // maz

#endif // !_H_C_FIXED_SIZE_STRING_
