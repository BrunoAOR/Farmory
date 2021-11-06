#ifndef _H_C_FILE_
#define _H_C_FILE_

#include <string>


namespace maz
{

class CFile
{
public:
    explicit CFile(const char* apFullPath);
    ~CFile();

    bool IsOk() const;
    const char* GetFullPath() const;

private:
    const char* mpPath;
};


class CTextData
{
public:
    static CTextData FromFile(const CFile& arFile);
    ~CTextData();

    bool IsOk() const;
    const char* GetData() const;

private:
    CTextData();
    explicit CTextData(const std::string& arData);

private:
    std::string mData;
};


class CImageData
{
public:
    static CImageData FromFile(const CFile& arFile);
    CImageData(CImageData&& arrOther);
    ~CImageData();

    bool IsOk() const;
    unsigned char* GetData() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetChannelsCount() const;

private:
    CImageData();
    explicit CImageData(unsigned char* apData);

private:
    unsigned char* mpData;
    int mWidth;
    int mHeight;
    int mChannelsCount;
};

} // maz

#endif // !_H_C_FILE_
