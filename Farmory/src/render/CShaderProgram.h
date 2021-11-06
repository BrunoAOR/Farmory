#ifndef _H_C_SHADER_PROGRAM_
#define _H_C_SHADER_PROGRAM_

#include <files/CFile.h>


namespace maz
{

class CShaderProgram
{
public:
    CShaderProgram();
    ~CShaderProgram();

    bool Init(const CFile& arVertexShaderFile, const CFile& arFragmentShaderFile);
    void End();

    bool IsOk() const;
    bool Use() const;

    static void StopUse();

    bool SetUniform1i(const char* apName, int aI);
    bool SetUniform1f(const char* apName, float aF);
    bool SetUniform4f(const char* apName, float aX, float aY, float aZ, float aW);
    bool SetUniformMat4(const char* apName, const float* apMat4Ptr);

private:
    unsigned int mId;
};

} // maz

#endif // !_H_C_SHADER_PROGRAM_
