#ifndef _H_ENUM_HELPERS_
#define _H_ENUM_HELPERS_


#define MAZ_ENUM_COUNT(enum_type) (static_cast<std::underlying_type_t<enum_type>>(enum_type::_Max) + 1)

template<typename ENUM_TYPE>
constexpr std::underlying_type_t<ENUM_TYPE> EnumToNumber(ENUM_TYPE aEnumValue)
{
    return static_cast<std::underlying_type_t<ENUM_TYPE>>(aEnumValue);
}


template<typename FLAG_ENUM_TYPE>
bool IsFlagSet(const FLAG_ENUM_TYPE& aFlags, FLAG_ENUM_TYPE aFlagToCheck)
{
    return ((EnumToNumber(aFlags) & EnumToNumber(aFlagToCheck)) == EnumToNumber(aFlagToCheck));
}


template<typename FLAG_ENUM_TYPE>
bool IsAnyFlagSet(const FLAG_ENUM_TYPE& aFlags, FLAG_ENUM_TYPE aFlagToCheck)
{
    return ((EnumToNumber(aFlags) & EnumToNumber(aFlagToCheck)) != 0);
}


template<typename FLAG_ENUM_TYPE>
void SetFlag(FLAG_ENUM_TYPE& aInOutFlags, FLAG_ENUM_TYPE aFlagToSet)
{
    aInOutFlags = static_cast<FLAG_ENUM_TYPE>(EnumToNumber(aInOutFlags) | EnumToNumber(aFlagToSet));
}


template<typename FLAG_ENUM_TYPE>
void ClearFlag(FLAG_ENUM_TYPE& aInOutFlags, FLAG_ENUM_TYPE aFlagToReset)
{
    aInOutFlags = static_cast<FLAG_ENUM_TYPE>(EnumToNumber(aInOutFlags) & (~EnumToNumber(aFlagToReset)));
}


template<typename FLAG_ENUM_TYPE>
void ToggleFlag(FLAG_ENUM_TYPE& aInOutFlags, FLAG_ENUM_TYPE aFlagToToggle)
{
    aInOutFlags = static_cast<FLAG_ENUM_TYPE>(EnumToNumber(aInOutFlags) ^ EnumToNumber(aFlagToToggle));
}

#endif // !_H_GLOBALS_
