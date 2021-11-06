#include <maz/globals.h>
#include <files/CFile.h>
#include <fstream>
#include <sstream>
#include <external/stb_image.h>


namespace maz
{

CFile::CFile(const char* apFullPath) : mpPath(apFullPath) { ; }


CFile::~CFile() { ; }


bool CFile::IsOk() const
{
	return (nullptr != mpPath);
}


const char* CFile::GetFullPath() const
{
    return mpPath;
}


CTextData CTextData::FromFile(const CFile& arFile)
{
	std::string lContent;
	std::string lLine;
	std::ifstream lFStream(arFile.GetFullPath(), std::ifstream::in);
	MAZ_LOGGER_IF(!lFStream.good(), "Failed to load text file at path '%s'", arFile);
	if (lFStream.good() && lFStream.is_open())
	{
		std::stringstream lSStream;
		lSStream << lFStream.rdbuf();
		lFStream.close();
		lContent = lSStream.str();

		lFStream.close();
	}

	return CTextData(lContent);
}


CTextData::CTextData() { ; }
CTextData::CTextData(const std::string& arData) : mData(arData) { ; }
CTextData::~CTextData() { ; }


bool CTextData::IsOk() const
{
    return !mData.empty();
}


const char* CTextData::GetData() const
{
    return mData.c_str();
}


CImageData CImageData::FromFile(const CFile& arFile)
{
	CImageData lImageData;
	if (arFile.IsOk())
	{
		stbi_set_flip_vertically_on_load(true);
		lImageData.mpData = stbi_load(arFile.GetFullPath(), &lImageData.mWidth, &lImageData.mHeight, &lImageData.mChannelsCount, 0);
		MAZ_LOGGER_IF(nullptr == lImageData.mpData, "Failed to load image file at path '%s'. Error: %s", arFile, stbi_failure_reason());
	}
	return lImageData;
}


CImageData::CImageData() : mpData(nullptr), mWidth(0), mHeight(0), mChannelsCount(0) { ; }
CImageData::CImageData(unsigned char* apData) : mpData(apData), mWidth(0), mHeight(0), mChannelsCount(0) { ; }


CImageData::CImageData(CImageData&& arrOther) : mpData(arrOther.mpData), mWidth(arrOther.mWidth), mHeight(arrOther.mHeight), mChannelsCount(arrOther.mChannelsCount)
{
	if (this != &arrOther)
	{ 
		arrOther.mpData = nullptr;
		arrOther.mWidth = 0;
		arrOther.mHeight = 0;
		arrOther.mChannelsCount = 0;
	}
}


CImageData::~CImageData()
{
    if (nullptr != mpData)
    {
        stbi_image_free(mpData);
        mpData = nullptr;
    }
}


bool CImageData::IsOk() const
{
	return (nullptr != mpData);
}


unsigned char* CImageData::GetData() const
{
    return mpData;
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
