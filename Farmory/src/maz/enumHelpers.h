#ifndef _H_ENUM_HELPERS_
#define _H_ENUM_HELPERS_


namespace maz
{

template<typename ENUM_TYPE>
constexpr std::underlying_type_t<ENUM_TYPE> EnumCount()
{
    return (static_cast<std::underlying_type_t<ENUM_TYPE>>(ENUM_TYPE::_Max) + 1);
}


template<typename ENUM_TYPE>
constexpr std::underlying_type_t<ENUM_TYPE> EnumToNumber(ENUM_TYPE aEnumValue)
{
    return static_cast<std::underlying_type_t<ENUM_TYPE>>(aEnumValue);
}


template<typename FLAG_ENUM_TYPE>
bool IsFlagSet(const FLAG_ENUM_TYPE& arFlags, FLAG_ENUM_TYPE aFlagToCheck)
{
    return ((EnumToNumber(arFlags) & EnumToNumber(aFlagToCheck)) == EnumToNumber(aFlagToCheck));
}


template<typename FLAG_ENUM_TYPE>
bool IsAnyFlagSet(const FLAG_ENUM_TYPE& arFlags, FLAG_ENUM_TYPE aFlagToCheck)
{
    return ((EnumToNumber(arFlags) & EnumToNumber(aFlagToCheck)) != 0);
}


template<typename FLAG_ENUM_TYPE>
void SetFlag(FLAG_ENUM_TYPE& arInOutFlags, FLAG_ENUM_TYPE aFlagToSet)
{
    arInOutFlags = static_cast<FLAG_ENUM_TYPE>(EnumToNumber(arInOutFlags) | EnumToNumber(aFlagToSet));
}


template<typename FLAG_ENUM_TYPE>
void ClearFlag(FLAG_ENUM_TYPE& arInOutFlags, FLAG_ENUM_TYPE aFlagToReset)
{
    arInOutFlags = static_cast<FLAG_ENUM_TYPE>(EnumToNumber(arInOutFlags) & (~EnumToNumber(aFlagToReset)));
}


template<typename FLAG_ENUM_TYPE>
void ToggleFlag(FLAG_ENUM_TYPE& arInOutFlags, FLAG_ENUM_TYPE aFlagToToggle)
{
    arInOutFlags = static_cast<FLAG_ENUM_TYPE>(EnumToNumber(arInOutFlags) ^ EnumToNumber(aFlagToToggle));
}

} // maz

#endif // !_H_GLOBALS_
