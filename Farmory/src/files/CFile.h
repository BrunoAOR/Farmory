#ifndef _H_C_FILE_
#define _H_C_FILE_

#include <string>


namespace maz
{
class CFile
{
public:
    explicit CFile(const char* aFullPath);
    ~CFile();

    bool IsOk() const;
    const char* GetFullPath() const;

private:
    const char* mPath;
};

class CTextData
{
public:
    static CTextData FromFile(const CFile& aFile);
    ~CTextData();

    bool IsOk() const;
    const char* GetData() const;

private:
    CTextData();
    explicit CTextData(const std::string& aData);

private:
    std::string mData;
};

class CImageData
{
public:
    static CImageData FromFile(const CFile& aFile);
    CImageData(CImageData&& aOther);
    ~CImageData();

    bool IsOk() const;
    unsigned char* GetData() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetChannelsCount() const;

private:
    CImageData();
    explicit CImageData(unsigned char* aData);

private:
    unsigned char* mData;
    int mWidth;
    int mHeight;
    int mChannelsCount;
};

} // maz

#endif // !_H_C_FILE_
