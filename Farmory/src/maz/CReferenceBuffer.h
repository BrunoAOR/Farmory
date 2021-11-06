#ifndef _H_C_REFERENCE_MASTER_BUFFER_
#define _H_C_REFERENCE_MASTER_BUFFER_
#define MAZ_LOG_VERBOSE

#include <maz/globals.h>
#include <maz/CReferenceMaster.h>


namespace maz
{

const uint16 kInvalidElementId = MAX_UINT16;

template<typename T, uint16 BUFFER_SIZE>
class CReferenceBuffer final
{
    static_assert(BUFFER_SIZE < kInvalidElementId);

public:
    CReferenceBuffer();
    ~CReferenceBuffer();
    void Clear();
    uint16 GetNextAvailableId();
    template<typename ... ConstructionArgs>
    uint16 AddElement(ConstructionArgs&& ... arrArgs);
    bool FlagElementForRemoval(const uint16 aElementId);
    bool RemoveFlaggedElement(const uint16 aElementId);
    CReference<T> GetElement(const uint16 aElementId);


public:
    enum EIteratorFlags
    {
        NONE                    = 0,
        ANY                     = 1 << 0,
        PROCESS_ADD_PENDING     = 1 << 1,
        PROCESS_REMOVE_PENDING  = 1 << 2
    };


private:
    enum class EBufferUseFlags : uint8
    {
        NONE        = 0,
        IN_USE      = 1 << 0,
        JUST_ADDED  = 1 << 1,
        TO_REMOVE   = 1 << 2
    };
    std::array<CReferenceHolder<T>, BUFFER_SIZE> mElements;
    uint8 mBuffer[sizeof(T) * BUFFER_SIZE];
    std::array<EBufferUseFlags, BUFFER_SIZE> mBufferUseFlags;
    // This is the first index after which (including itself) no more elements are flagged as IN_USE
    // Note that there may still be elements NOT IN_USE before.
    uint16 mStartOfInactiveRange;


public:
    class CBufferIterator
    {
    public:
        operator bool();
        void operator++();
        bool HasIteratorFlag(EIteratorFlags aIteratorFlag);
        void ClearIteratorFlag(EIteratorFlags aIteratorFlag);
        CReference<T> Get();
        uint16 GetId();
        bool RemoveElement();

    private:
        friend class CReferenceBuffer;
        CBufferIterator(CReferenceBuffer* apManager, EBufferUseFlags aFlags);
        void findNextValidIndex(const uint16 aStartIndex);

        CReferenceBuffer* mpManager;
        EBufferUseFlags mFlags;
        uint16 mCurrentIndex;
    };

    CBufferIterator GetIterator(EIteratorFlags aFlags);

private:
    EBufferUseFlags iteratorToBufferFlags(EIteratorFlags aIteratorFlags);
};


template<typename T, uint16 BUFFER_SIZE>
inline CReferenceBuffer<T, BUFFER_SIZE>::CReferenceBuffer()
{
    MAZ_LOGGER_VERBOSE("Called");
    for (size_t i = 0, iCount = mElements.size(); i < iCount; ++i)
    {
        mElements[i] = std::move(CReferenceHolder<T>());
    }
    std::memset(mBuffer, 0, sizeof(T) * BUFFER_SIZE);
    mBufferUseFlags.fill(EBufferUseFlags::NONE);
    mStartOfInactiveRange = 0u;
}


template<typename T, uint16 BUFFER_SIZE>
inline CReferenceBuffer<T, BUFFER_SIZE>::~CReferenceBuffer()
{
    MAZ_LOGGER_VERBOSE("Called");
    Clear();
}


template<typename T, uint16 BUFFER_SIZE>
inline void CReferenceBuffer<T, BUFFER_SIZE>::Clear()
{
    for (uint16 i = 0; i < mStartOfInactiveRange; ++i)
    {
        if (IsFlagSet(mBufferUseFlags[i], EBufferUseFlags::IN_USE))
        {
            mElements[i].~CReferenceHolder<T>();
            reinterpret_cast<T*>(&(mBuffer[sizeof(T) * i]))->~T();
            mBufferUseFlags[i] = EBufferUseFlags::NONE;
        }
    }

    mStartOfInactiveRange = 0u;
}


template<typename T, uint16 BUFFER_SIZE>
inline uint16 CReferenceBuffer<T, BUFFER_SIZE>::GetNextAvailableId()
{
    uint16 lElementId = kInvalidElementId;
    for (uint16 i = 0, iCount = static_cast<uint16>(mBufferUseFlags.size()); (i < iCount) && (lElementId == kInvalidElementId); ++i)
    {
        if (!IsFlagSet(mBufferUseFlags[i], EBufferUseFlags::IN_USE))
        {
            lElementId = i;
        }
    }
    MAZ_ASSERT(lElementId <= mStartOfInactiveRange, "The elementId to return (%hu) is greater than mStartOfInactiveRange (%hu)!", lElementId, mStartOfInactiveRange);
    return lElementId;
}


template<typename T, uint16 BUFFER_SIZE>
template<typename ...ConstructionArgs>
inline uint16 CReferenceBuffer<T, BUFFER_SIZE>::AddElement(ConstructionArgs&& ...arrArgs)
{
    uint16 lElementId = kInvalidElementId;
    for (uint16 i = 0, iCount = static_cast<uint16>(mBufferUseFlags.size()); (i < iCount) && (lElementId == kInvalidElementId); ++i)
    {
        if (!IsFlagSet(mBufferUseFlags[i], EBufferUseFlags::IN_USE))
        {
            SetFlag(mBufferUseFlags[i], EBufferUseFlags::IN_USE);
            SetFlag(mBufferUseFlags[i], EBufferUseFlags::JUST_ADDED);
            lElementId = i;
        }
    }
    MAZ_ASSERT(lElementId != kInvalidElementId, "Failed to find an available slot for element!");
    if (lElementId != kInvalidElementId)
    {
        mElements[lElementId] = CReferenceHolder<T>(MAZ_PLACEMENT_NEW(&(mBuffer[sizeof(T) * lElementId]), T, std::forward<ConstructionArgs>(arrArgs) ...));
        MAZ_ASSERT(lElementId <= mStartOfInactiveRange, "The elementId to return (%hu) is greater than mStartOfInactiveRange (%hu)!", lElementId, mStartOfInactiveRange);
        if (lElementId == mStartOfInactiveRange)
        {
            ++mStartOfInactiveRange;
        }
    }

    return lElementId;
}


template<typename T, uint16 BUFFER_SIZE>
inline bool CReferenceBuffer<T, BUFFER_SIZE>::FlagElementForRemoval(const uint16 aElementId)
{
    const bool lOk = IsFlagSet(mBufferUseFlags[aElementId], EBufferUseFlags::IN_USE);

    if (lOk)
    {
        MAZ_ASSERT(!IsFlagSet(mBufferUseFlags[aElementId], EBufferUseFlags::JUST_ADDED), "Attempting to remove an element that has been added and remains flagged as pending!");
        SetFlag(mBufferUseFlags[aElementId], EBufferUseFlags::TO_REMOVE);
    }

    return lOk;
}


template<typename T, uint16 BUFFER_SIZE>
inline bool CReferenceBuffer<T, BUFFER_SIZE>::RemoveFlaggedElement(const uint16 aElementId)
{
    const bool lOk = (IsFlagSet(mBufferUseFlags[aElementId], EBufferUseFlags::TO_REMOVE));
    if (lOk)
    {
        MAZ_ASSERT(IsFlagSet(mBufferUseFlags[aElementId], EBufferUseFlags::IN_USE), "Attempting to remove an element (id %hhu) which is flagged with TO_REMOVE but not with IN_USE!", aElementId);
        ClearFlag(mBufferUseFlags[aElementId], EBufferUseFlags::TO_REMOVE);
        mElements[aElementId].~CReferenceHolder<T>();
        reinterpret_cast<T*>(&(mBuffer[sizeof(T) * aElementId]))->~T();
        ClearFlag(mBufferUseFlags[aElementId], EBufferUseFlags::IN_USE);
        
        if (aElementId == mStartOfInactiveRange - 1)
        {
            // So, we've removed the element IN_USE that was last in the list. We go backwards looking for the new last element. 
            // We set mStartOfInactiveRange to zero so that if no element is flagged as IN_USE, we already have the right value.
            mStartOfInactiveRange = 0;
            for (uint16 i = aElementId; (i > 0) && (mStartOfInactiveRange == 0); --i)
            {
                const uint16 lQueryIndex = i - 1; // This is NOT done in the for-increment to prevent i from overflowing through 0 to MAX_UINT16. We start before aElementId.

                if (IsFlagSet(mBufferUseFlags[lQueryIndex], EBufferUseFlags::IN_USE))
                {
                    mStartOfInactiveRange = lQueryIndex + 1;
                }
            }
            // Note that if no element had the IN_USE flag, then mStartOfInactiveRange will remain as 0, which is correct.
        }
    }

    return lOk;
}


template<typename T, uint16 BUFFER_SIZE>
inline CReference<T> CReferenceBuffer<T, BUFFER_SIZE>::GetElement(const uint16 aElementId)
{
    MAZ_ASSERT(IsFlagSet(mBufferUseFlags[aElementId], EBufferUseFlags::IN_USE), "Attempting to retrieve uninitialized element with id %hu!", aElementId);
    return (IsFlagSet(mBufferUseFlags[aElementId], EBufferUseFlags::IN_USE)) ? mElements[aElementId].GetReference() : CReference<T>();
}


template<typename T, uint16 BUFFER_SIZE>
inline typename CReferenceBuffer<T, BUFFER_SIZE>::CBufferIterator CReferenceBuffer<T, BUFFER_SIZE>::GetIterator(EIteratorFlags aFlags)
{
    EBufferUseFlags lBufferFlags = iteratorToBufferFlags(aFlags);
    return CBufferIterator(this, lBufferFlags);
}


template<typename T, uint16 BUFFER_SIZE>
inline typename CReferenceBuffer<T, BUFFER_SIZE>::EBufferUseFlags CReferenceBuffer<T, BUFFER_SIZE>::iteratorToBufferFlags(EIteratorFlags aIteratorFlags)
{
    EBufferUseFlags lBufferFlags = EBufferUseFlags::NONE;
    if (IsFlagSet(aIteratorFlags, EIteratorFlags::ANY))
    {
        // If ANY is set, that means we don't care about any pending status, so we return an iterator to all valid Elements (the ones flagged as IN_USE)
        SetFlag(lBufferFlags, EBufferUseFlags::IN_USE);
    }
    else
    {
        if (IsFlagSet(aIteratorFlags, EIteratorFlags::PROCESS_ADD_PENDING))
        {
            SetFlag(lBufferFlags, EBufferUseFlags::JUST_ADDED);
        }
        if (IsFlagSet(aIteratorFlags, EIteratorFlags::PROCESS_REMOVE_PENDING))
        {
            SetFlag(lBufferFlags, EBufferUseFlags::TO_REMOVE);
        }
    }
    return lBufferFlags;
}


// **********
// **********
// ITERATOR
// **********
// **********


template<typename T, uint16 BUFFER_SIZE>
inline CReferenceBuffer<T, BUFFER_SIZE>::CBufferIterator::operator bool()
{
    return (mCurrentIndex != kInvalidElementId) && static_cast<bool>(mpManager->mElements[mCurrentIndex]);
}


template<typename T, uint16 BUFFER_SIZE>
inline void CReferenceBuffer<T, BUFFER_SIZE>::CBufferIterator::operator++()
{
    if (mCurrentIndex != kInvalidElementId)
    {
        findNextValidIndex(mCurrentIndex + 1);
    }
}


template<typename T, uint16 BUFFER_SIZE>
inline bool CReferenceBuffer<T, BUFFER_SIZE>::CBufferIterator::HasIteratorFlag(EIteratorFlags aIteratorFlag)
{
    const EBufferUseFlags lBufferFlags = mpManager->iteratorToBufferFlags(aIteratorFlag);
    return IsFlagSet(mpManager->mBufferUseFlags[mCurrentIndex], lBufferFlags);
}


template<typename T, uint16 BUFFER_SIZE>
inline void CReferenceBuffer<T, BUFFER_SIZE>::CBufferIterator::ClearIteratorFlag(EIteratorFlags aIteratorFlag)
{
    const EBufferUseFlags lBufferFlags = mpManager->iteratorToBufferFlags(aIteratorFlag);
    ClearFlag(mpManager->mBufferUseFlags[mCurrentIndex], lBufferFlags);
}


template<typename T, uint16 BUFFER_SIZE>
inline CReference<T> CReferenceBuffer<T, BUFFER_SIZE>::CBufferIterator::Get()
{
    return mpManager->mElements[mCurrentIndex].GetReference();
}


template<typename T, uint16 BUFFER_SIZE>
inline uint16 CReferenceBuffer<T, BUFFER_SIZE>::CBufferIterator::GetId()
{
    return mCurrentIndex;
}


template<typename T, uint16 BUFFER_SIZE>
inline bool CReferenceBuffer<T, BUFFER_SIZE>::CBufferIterator::RemoveElement()
{
    return mpManager->RemoveFlaggedElement(mCurrentIndex);
}


template<typename T, uint16 BUFFER_SIZE>
inline CReferenceBuffer<T, BUFFER_SIZE>::CBufferIterator::CBufferIterator(CReferenceBuffer* apManager, EBufferUseFlags aFlags)
    : mpManager(apManager)
    , mFlags(aFlags)
    , mCurrentIndex(0)
{
    MAZ_ASSERT(mpManager != nullptr, "Attempting to construct a CBufferIterator without providing a valid CReferenceBuffer pointer!");
    findNextValidIndex(0);
}


template<typename T, uint16 BUFFER_SIZE>
inline void CReferenceBuffer<T, BUFFER_SIZE>::CBufferIterator::findNextValidIndex(const uint16 aStartIndex)
{
    mCurrentIndex = kInvalidElementId;
    for (uint16 i = aStartIndex; (i < mpManager->mStartOfInactiveRange) && (mCurrentIndex == kInvalidElementId); ++i)
    {
        if(IsFlagSet(mpManager->mBufferUseFlags[i], EBufferUseFlags::IN_USE) && IsAnyFlagSet(mpManager->mBufferUseFlags[i], mFlags))
        {
            mCurrentIndex = i;
        }
    }
}

} // maz

#endif // !_H_C_REFERENCE_MASTER_BUFFER_
