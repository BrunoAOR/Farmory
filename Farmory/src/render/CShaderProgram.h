#ifndef _H_C_SHADER_PROGRAM_
#define _H_C_SHADER_PROGRAM_

#include <files/CFile.h>


struct GLFWwindow;
namespace maz
{

class CShaderProgram
{
public:
    CShaderProgram();
    ~CShaderProgram();

    bool Init(const CFile& aVertexShaderFile, const CFile& aFragmentShaderFile);
    void End();

    bool IsOk() const;
    bool Use() const;

    static void StopUse();

    bool SetUniform1i(const char* aName, int aI);
    bool SetUniform1f(const char* aName, float aF);
    bool SetUniform4f(const char* aName, float aX, float aY, float aZ, float aW);
    bool SetUniformMat4(const char* aName, float* aMat4Ptr);

private:
    unsigned int mId;
};

} // maz

#endif // !_H_C_SHADER_PROGRAM_
