#include <maz/globals.h>
#include <render/CShaderProgram.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>


namespace maz
{

namespace
{

unsigned int BuildShader(const char* aSource, GLenum aType)
{
    unsigned int lShader = glCreateShader(aType);
    glShaderSource(lShader, 1, &aSource, GL_NONE);
    glCompileShader(lShader);

    int lOk;
    glGetShaderiv(lShader, GL_COMPILE_STATUS, &lOk);

    if (!lOk)
    {
        char lInfoBuffer[512];
        glGetShaderInfoLog(lShader, sizeof(lInfoBuffer), GL_NONE, lInfoBuffer);
        MAZ_LOGGER("[CShaderProgram] BuildShader - Shader compilation failed. Error: %s", lInfoBuffer);
        glDeleteShader(lShader);
        lShader = GL_NONE;
    }

    return lShader;
}

} // anonymous


CShaderProgram::CShaderProgram() : mId(GL_NONE) { ; }


CShaderProgram::~CShaderProgram()
{
    MAZ_ASSERT(!IsOk(), "[CShaderProgram]::~CShaderProgram - Destructor called on an initialized ShaderProgram without having called 'End'!");
}


bool CShaderProgram::Init(const CFile& aVertexShaderFile, const CFile& aFragmentShaderFile)
{
    bool lOk = true;

    if (IsOk())
    {
        End();
    }

    CTextData lVertexSource = CTextData::FromFile(aVertexShaderFile);
    CTextData lFragmentSource = CTextData::FromFile(aFragmentShaderFile);
    lOk = lOk && lVertexSource.IsOk() && lFragmentSource.IsOk();

    if (lOk)
    {
        unsigned int lVertexShader = BuildShader(lVertexSource.GetData(), GL_VERTEX_SHADER);
        MAZ_LOGGER_IF(GL_NONE == lVertexShader, "[CShaderProgram]::Init - Compilation of Vertex Shader failed. Source was: \n%s", lVertexSource.GetData());
        unsigned int lFragmentShader = BuildShader(lFragmentSource.GetData(), GL_FRAGMENT_SHADER);
        MAZ_LOGGER_IF(GL_NONE == lFragmentShader, "[CShaderProgram]::Init - Compilation of Fragment Shader failed. Source was: \n%s", lFragmentSource.GetData());
        lOk = lOk && (GL_NONE != lVertexShader) && (GL_NONE != lFragmentShader);

        if (lOk)
        {
            unsigned int lProgram = glCreateProgram();
            glAttachShader(lProgram, lVertexShader);
            glAttachShader(lProgram, lFragmentShader);
            glLinkProgram(lProgram);

            int lProgOk;
            glGetProgramiv(lProgram, GL_LINK_STATUS, &lProgOk);
            lOk = lOk && lProgOk;
            if (lOk)
            {
                mId = lProgram;
            }
            else
            {
                char lInfoBuffer[512];
                glGetProgramInfoLog(lProgram, sizeof(lInfoBuffer), GL_NONE, lInfoBuffer);
                MAZ_LOGGER("[CShaderProgram]::Init - Shader Program failed to link. Error: %s", lInfoBuffer);
                glDeleteProgram(lProgram);
                lProgram = GL_NONE;
            }
        }
        glDeleteShader(lVertexShader);
        glDeleteShader(lFragmentShader);
    }

    return lOk;
}


void CShaderProgram::End()
{
    if (GL_NONE != mId)
    {
        glDeleteProgram(mId);
        mId = GL_NONE;
    }
}


bool CShaderProgram::IsOk() const
{
    return (GL_NONE != mId);
}


bool CShaderProgram::Use() const
{
    bool lOk = IsOk();
    MAZ_ASSERT(lOk, "[CShaderProgram]::Use - Failed due to CShaderProgram not being correctly initialized!");
    if (lOk)
    {
        glUseProgram(mId);
    }
    return lOk;
}


void CShaderProgram::StopUse()
{
    glUseProgram(GL_NONE);
}


bool CShaderProgram::SetUniform1i(const char* aName, int aI)
{
    bool lOk = IsOk();
    if (lOk)
    {
        int lUniformLocation = glGetUniformLocation(mId, aName);
        lOk = lOk && (-1 != lUniformLocation);
        if (lOk)
        {
            Use();
            glUniform1i(lUniformLocation, aI);
        }
    }

    return lOk;
}


bool CShaderProgram::SetUniform1f(const char* aName, float aF)
{
    bool lOk = IsOk();
    if (lOk)
    {
        int lUniformLocation = glGetUniformLocation(mId, aName);
        lOk = lOk && (-1 != lUniformLocation);
        if (lOk)
        {
            Use();
            glUniform1f(lUniformLocation, aF);
        }
    }

    return lOk;
}


bool CShaderProgram::SetUniform4f(const char* aName, float aX, float aY, float aZ, float aW)
{
    bool lOk = IsOk();
    if (lOk)
    {
        int lUniformLocation = glGetUniformLocation(mId, aName);
        lOk = lOk && (-1 != lUniformLocation);
        if (lOk)
        {
            Use();
            glUniform4f(lUniformLocation, aX, aY, aZ, aW);
        }
    }

    return lOk;
}


bool CShaderProgram::SetUniformMat4(const char* aName, const float* aMat4Ptr)
{
    bool lOk = IsOk();
    if (lOk)
    {
        int lUniformLocation = glGetUniformLocation(mId, aName);
        lOk = lOk && (-1 != lUniformLocation);
        if (lOk)
        {
            Use();
            glUniformMatrix4fv(lUniformLocation, 1, GL_FALSE, aMat4Ptr);
        }
    }

    return lOk;
}

} // maz
