#include <maz/globals.h>
#include <render/CTexture2D.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>


namespace maz
{
namespace
{
const uint32 kTextureUnitsCount = 16;
const GLint kAxisMap[]          = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T };
const GLint kWrapModeMap[]      = { GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER };
const GLint kMinFilterModeMap[] = { GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR };
const GLint kMagFilterModeMap[] = { GL_NEAREST, GL_LINEAR };

} // anonymous


CTexture2D::CTexture2D()
    : mId(GL_NONE)
    , mCurrentTextureUnit(MAX_UINT32)
    , mXWrapMode(EWrapMode::REPEAT)
    , mYWrapMode(EWrapMode::REPEAT)
    , mMinFilterMode(EMinFilterMode::NEAREST)
    , mMagFilterMode(EMagFilterMode::NEAREST)
{ ; }

CTexture2D::~CTexture2D()
{
    MAZ_ASSERT(!IsOk(), "[CTexture2D]::~CTexture2D - Destructor called on an initialized Texture2D without having called 'End'!");
}


bool CTexture2D::Init()
{
    if (IsOk())
    {
        End();
    }
    glGenTextures(1, &mId);
    return IsOk();
}


void CTexture2D::End()
{
    if (GL_NONE != mId)
    {
        glDeleteTextures(1, &mId);
        mId = GL_NONE;
    }
    mCurrentTextureUnit = MAX_UINT32;
    mXWrapMode = EWrapMode::REPEAT;
    mYWrapMode = EWrapMode::REPEAT;
    mMinFilterMode = EMinFilterMode::NEAREST;
    mMagFilterMode = EMagFilterMode::NEAREST;
}


bool CTexture2D::IsOk() const
{
    return (GL_NONE != mId);
}


bool CTexture2D::SetImage(const CFile& aImageFile, bool aGenerateMipmaps)
{
    bool lOk = true;
    lOk = lOk && IsOk();
    lOk = lOk && aImageFile.IsOk();

    if (lOk)
    {
        CImageData lData = CImageData::FromFile(aImageFile);
        lOk = lOk && lData.IsOk();
        if (lOk)
        {
            glBindTexture(GL_TEXTURE_2D, mId);

            // Texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, kWrapModeMap[static_cast<uint32>(mXWrapMode)]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, kWrapModeMap[static_cast<uint32>(mYWrapMode)]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, kMinFilterModeMap[static_cast<uint32>(mMinFilterMode)]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, kMagFilterModeMap[static_cast<uint32>(mMagFilterMode)]);

            const GLint lFormat = (3 == lData.GetChannelsCount()) ? GL_RGB : GL_RGBA; //GL_RGB: 6407 // GL_RGBA: 6408
            glTexImage2D(GL_TEXTURE_2D, 0, lFormat, lData.GetWidth(), lData.GetHeight(), 0, lFormat, GL_UNSIGNED_BYTE, lData.GetData());
            if (aGenerateMipmaps)
            {
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            glBindTexture(GL_TEXTURE_2D, GL_NONE);
        }
    }
    return lOk;
}


EWrapMode CTexture2D::GetWrapMode(EAxis aAxis) const
{
    if (EAxis::X == aAxis)
    {
        return mXWrapMode;
    }
    else if(EAxis::Y == aAxis)
    {
        return mYWrapMode;
    }
    MAZ_ASSERT(false, "[CTexture2D]::GetWrapMode - Invalid axis received as argument. Will return 'EWrapMode::REPEAT'!");
    return EWrapMode::REPEAT;
}


EMinFilterMode CTexture2D::GetMinFilterMode() const
{
    return mMinFilterMode;
}


EMagFilterMode CTexture2D::GetMagFiletrMode() const
{
    return mMagFilterMode;
}


bool CTexture2D::SetWrapMode(EAxis aAxis, EWrapMode aWrapMode)
{
    bool lOk = IsOk();
    MAZ_ASSERT(lOk, "[CTexture2D]::SetWrapMode - Failed due to CTexture2D not being initialized!");
    if (lOk)
    {
        if (EAxis::X == aAxis)
        {
            mXWrapMode = aWrapMode;
            glBindTexture(GL_TEXTURE_2D, mId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, kWrapModeMap[static_cast<uint32>(mXWrapMode)]);
            glBindTexture(GL_TEXTURE_2D, GL_NONE);
        }
        else if (EAxis::Y == aAxis)
        {
            mYWrapMode = aWrapMode;
            glBindTexture(GL_TEXTURE_2D, mId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, kWrapModeMap[static_cast<uint32>(mYWrapMode)]);
            glBindTexture(GL_TEXTURE_2D, GL_NONE);
        }
    }
    return lOk;
}


bool CTexture2D::SetMinFilterMode(EMinFilterMode aMinFilterMode)
{
    bool lOk = IsOk();
    MAZ_ASSERT(lOk, "[CTexture2D]::SetMinFilterMode - Failed due to CTexture2D not being initialized!");
    if (lOk)
    {
        mMinFilterMode = aMinFilterMode;
        glBindTexture(GL_TEXTURE_2D, mId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, kMinFilterModeMap[static_cast<uint32>(mMinFilterMode)]);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }
    return lOk;
}


bool CTexture2D::SetMagFilterMode(EMagFilterMode aMagFilterMode)
{
    bool lOk = IsOk();
    MAZ_ASSERT(lOk, "[CTexture2D]::SetMagFilterMode - Failed due to CTexture2D not being initialized!");
    if (lOk)
    {
        mMagFilterMode = aMagFilterMode;
        glBindTexture(GL_TEXTURE_2D, mId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, kMagFilterModeMap[static_cast<uint32>(mMagFilterMode)]);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }
    return lOk;
}


bool CTexture2D::BindTexture(uint32 aTextureUnit)
{
    bool lOk = true;
    lOk = lOk && IsOk();
    lOk = lOk && (aTextureUnit < kTextureUnitsCount - 1);
    if (lOk)
    {
        UnbindTexture();
        mCurrentTextureUnit = aTextureUnit;
        glActiveTexture(GL_TEXTURE0 + mCurrentTextureUnit);
        glBindTexture(GL_TEXTURE_2D, mId);
    }
    return lOk;
}


void CTexture2D::UnbindTexture()
{
    if (MAX_UINT32 != mCurrentTextureUnit)
    {
        glActiveTexture(GL_TEXTURE0 + mCurrentTextureUnit);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }
}


bool CTexture2D::Apply(const CImageData& aImageData)
{
    bool lOk = true;
    lOk = lOk && (GL_NONE != mId);
    lOk = lOk && aImageData.IsOk();
    lOk = lOk && ((3 == aImageData.GetChannelsCount()) || (4 == aImageData.GetChannelsCount()));

    if (lOk)
    {
        // Texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, kWrapModeMap[static_cast<uint32>(mXWrapMode)]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, kWrapModeMap[static_cast<uint32>(mYWrapMode)]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, kMinFilterModeMap[static_cast<uint32>(mMinFilterMode)]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, kMagFilterModeMap[static_cast<uint32>(mMagFilterMode)]);

        const GLint lFormat = (3 == aImageData.GetChannelsCount()) ? GL_RGB : GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, lFormat, aImageData.GetWidth(), aImageData.GetHeight(), 0, lFormat, GL_UNSIGNED_BYTE, aImageData.GetData());
        //if (mAutoMipmaps)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    return lOk;
}

} // maz
