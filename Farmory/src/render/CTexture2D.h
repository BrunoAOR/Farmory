#ifndef _H_C_TEXTURE_
#define _H_C_TEXTURE_

#include <files/CFile.h>


namespace maz
{

enum class EAxis : uint32;
enum class EWrapMode : uint32;
enum class EMinFilterMode : uint32;
enum class EMagFilterMode : uint32;


class CTexture2D
{
public:
    CTexture2D();
    ~CTexture2D();

    bool Init();
    void End();

    bool IsOk() const;

    bool SetImage(const CFile& aImageFile, bool aGenerateMipmaps);
    
    EWrapMode GetWrapMode(EAxis aAxis) const;
    EMinFilterMode GetMinFilterMode() const;
    EMagFilterMode GetMagFiletrMode() const;

    bool SetWrapMode(EAxis aAxis, EWrapMode aWrapMode);
    bool SetMinFilterMode(EMinFilterMode aMinFilterMode);
    bool SetMagFilterMode(EMagFilterMode aMagFilterMode);

    bool BindTexture(uint32 aTextureUnit);
    void UnbindTexture();

private:
    bool Apply(const CImageData& aImageData);

private:
    uint32 mId;

    uint32 mCurrentTextureUnit;

    EWrapMode mXWrapMode;
    EWrapMode mYWrapMode;
    EMinFilterMode mMinFilterMode;
    EMagFilterMode mMagFilterMode;
};

enum class EAxis : uint32
{
    X,
    Y
};

enum class EWrapMode : uint32
{
    REPEAT,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER
};

enum class EMinFilterMode : uint32
{
    NEAREST,
    LINEAR,
    NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMANP_LINEAR,
    LINEAR_MIPMAP_LINEAR
};

enum class EMagFilterMode : uint32
{
    NEAREST,
    LINEAR
};

} // maz

#endif // !_H_C_TEXTURE_
