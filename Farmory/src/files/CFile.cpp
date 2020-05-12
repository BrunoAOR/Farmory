#include <maz/globals.h>
#include <files/CFile.h>
#include <fstream>
#include <sstream>
#include <external/stb_image.h>

namespace maz
{

CFile::CFile(const char* aFullPath) : mPath(aFullPath) { ; }
CFile::~CFile() { ; }

bool CFile::IsOk() const
{
	return (nullptr != mPath);
}

const char* CFile::GetFullPath() const
{
    return mPath;
}


CTextData CTextData::FromFile(const CFile& aFile)
{
	std::string lContent;
	std::string lLine;
	std::ifstream lFStream(aFile.GetFullPath(), std::ifstream::in);
	MAZ_LOGGER_IF(!lFStream.good(), "[CTextData]::FromFile - Failed to load text file at path '%s'", aFile);
	if (lFStream.good() && lFStream.is_open())
	{
		std::stringstream lSStream;
		lSStream << lFStream.rdbuf();
		lFStream.close();
		lContent = lSStream.str();
	}

	return CTextData(lContent);
}


CTextData::CTextData() { ; }
CTextData::CTextData(const std::string& aData) : mData(aData) { ; }
CTextData::~CTextData() { ; }

bool CTextData::IsOk() const
{
    return !mData.empty();
}

const char* CTextData::GetData() const
{
    return mData.c_str();
}


CImageData CImageData::FromFile(const CFile& aFile)
{
	CImageData lImageData;
	if (aFile.IsOk())
	{
		stbi_set_flip_vertically_on_load(true);
		lImageData.mData = stbi_load(aFile.GetFullPath(), &lImageData.mWidth, &lImageData.mHeight, &lImageData.mChannelsCount, 0);
		MAZ_LOGGER_IF(nullptr == lImageData.mData, "[CImageData]::FromFile - Failed to load image file at path '%s'. Error: %s", aFile, stbi_failure_reason());
	}
	return lImageData;
}

CImageData::CImageData() : mData(nullptr), mWidth(0), mHeight(0), mChannelsCount(0) { ; }
CImageData::CImageData(unsigned char* aData) : mData(aData), mWidth(0), mHeight(0), mChannelsCount(0) { ; }
CImageData::CImageData(CImageData&& aOther) : mData(aOther.mData), mWidth(aOther.mWidth), mHeight(aOther.mHeight), mChannelsCount(aOther.mChannelsCount)
{
	if (this != &aOther)
	{ 
		aOther.mData = nullptr;
		aOther.mWidth = 0;
		aOther.mHeight = 0;
		aOther.mChannelsCount = 0;
	}
}

CImageData::~CImageData()
{
    if (nullptr != mData)
    {
        stbi_image_free(mData);
        mData = nullptr;
    }
}

bool CImageData::IsOk() const
{
	return (nullptr != mData);
}

unsigned char* CImageData::GetData() const
{
    return mData;
}

int CImageData::GetWidth() const
{
	return mWidth;
}

int CImageData::GetHeight() const
{
	return mHeight;
}

int CImageData::GetChannelsCount() const
{
	return mChannelsCount;
}

} // maz
