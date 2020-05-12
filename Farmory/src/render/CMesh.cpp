#include <maz/globals.h>
#include <render/CMesh.h>
#include <GLAD/glad.h>


namespace maz
{

CMesh::CMesh()
    : mVAO         (GL_NONE)
    , mVBO         (GL_NONE)
    , mEBO         (GL_NONE)
    ,mIndexesCount (0)
{ ; }


CMesh::~CMesh()
{
    MAZ_ASSERT(!IsOk(), "[CMesh]::~CMesh - Destructor called on an initialized Mesh without having called 'End'!");
}


bool CMesh::InitRectangle(uint32 aWidth, uint32 aHeight)
{
    MAZ_DBG_UNUSED_VAR(aWidth);
    MAZ_DBG_UNUSED_VAR(aHeight);
    bool lOk = true;

    if (IsOk())
    {
        End();
    }

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);
    
    lOk = lOk && IsOk();

    if (lOk)
    {
        float lVertices[] = {
            // positions            // UV-coords
            -0.5f,  0.5f, 0.0f,    0.0f, 2.0f, // top left 
            -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, // bottom left
             0.5f, -0.5f, 0.0f,    2.0f, 0.0f, // bottom right
             0.5f,  0.5f, 0.0f,    2.0f, 2.0f  // top right
        };

        unsigned int lIndices[] = {
            0, 1, 2,
            2, 3, 0
        };

        mIndexesCount = sizeof(lIndices) / sizeof(unsigned int);

        glBindVertexArray(mVAO);

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lVertices), lVertices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0 * sizeof(float)));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lIndices), lIndices, GL_STATIC_DRAW);

        glBindVertexArray(GL_NONE);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
    }
    return lOk;
}


bool CMesh::InitTriangle(uint32 aWidth, uint32 aHeight)
{
    MAZ_DBG_UNUSED_VAR(aWidth);
    MAZ_DBG_UNUSED_VAR(aHeight);
    bool lOk = true;

    if (IsOk())
    {
        End();
    }

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    lOk = lOk && IsOk();

    if (lOk)
    {
        float lVertices[] = {
            // Positions            // Colors
            -0.25f, -0.75f, 0.0f,   1.0f, 0.0f, 0.0f, // bottom left
             0.25f, -0.75f, 0.0f,   0.0f, 1.0f, 0.0f, // bottom right
             0.00f, -0.25f, 0.0f,   0.0f, 0.0f, 1.0f  // top center
        };

        unsigned int lIndices[] = {
            0, 1, 2
        };

        mIndexesCount = sizeof(lIndices) / sizeof(unsigned int);

        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lVertices), lVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0 * sizeof(float)));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lIndices), lIndices, GL_STATIC_DRAW);

        glBindVertexArray(GL_NONE);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
    }
    return lOk;
}


bool CMesh::InitCube()
{
    bool lOk = true;

    if (IsOk())
    {
        End();
    }

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    lOk = lOk && IsOk();

    if (lOk)
    {
        const float lHalfSize = 0.5f;
        float lVertices[] = {
            // positions                        // UV-coords
             lHalfSize, -lHalfSize, -lHalfSize, 0.0f, 0.0f, //  0 - front bottom left
             lHalfSize, -lHalfSize,  lHalfSize, 2.0f, 0.0f, //  1 - front bottom right
             lHalfSize,  lHalfSize,  lHalfSize, 2.0f, 2.0f, //  2 - front top    right
             lHalfSize,  lHalfSize, -lHalfSize, 0.0f, 2.0f, //  3 - front top    left
            -lHalfSize, -lHalfSize, -lHalfSize, 2.0f, 0.0f, //  4 - back  bottom left
            -lHalfSize, -lHalfSize,  lHalfSize, 0.0f, 0.0f, //  5 - back  bottom right
            -lHalfSize,  lHalfSize,  lHalfSize, 0.0f, 2.0f, //  6 - back  top    right
            -lHalfSize,  lHalfSize, -lHalfSize, 2.0f, 2.0f, //  7 - back  top    left
            //Repeated vertices to allow for proper UV coords at bottom face
             lHalfSize, -lHalfSize, -lHalfSize, 2.0f, 2.0f, //  8 - front bottom left
             lHalfSize, -lHalfSize,  lHalfSize, 0.0f, 2.0f, //  9 - front bottom right
            //Repeated vertices to allow for proper UV coords at top faces
             lHalfSize,  lHalfSize,  lHalfSize, 0.0f, 0.0f, // 10 - front top    right
             lHalfSize,  lHalfSize, -lHalfSize, 2.0f, 0.0f  // 11 - front top    left
        };

        unsigned int lIndices[]
        {
             0,  1,  2,    2,  3,  0, // front  face
             5,  4,  7,    7,  6,  5, // back   face
             1,  5,  6,    6,  2,  1, // right  face
             4,  0,  3,    3,  7,  4, // left   face
             4,  5,  9,    9,  8,  4, // bottom face
            11, 10,  6,    6,  7, 11  // top    face
        };

        mIndexesCount = sizeof(lIndices) / sizeof(unsigned int);

        glBindVertexArray(mVAO);

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lVertices), lVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast< void* >(0 * sizeof(float)));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast< void* >(3 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lIndices), lIndices, GL_STATIC_DRAW);

        glBindVertexArray(GL_NONE);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
    }

    return lOk;
}


void CMesh::End()
{
    if (IsOk())
    {
        glDeleteVertexArrays(1, &mVAO);
        glDeleteBuffers(1, &mVBO);
        glDeleteBuffers(1, &mEBO);
        mVAO = GL_NONE;
        mVBO = GL_NONE;
        mEBO = GL_NONE;
    }
}


bool CMesh::IsOk() const
{
    return ((GL_NONE != mVAO) && (GL_NONE != mVBO) && (GL_NONE != mEBO));
}


bool CMesh::Draw() const
{
    bool lOk = IsOk();

    if (lOk)
    {
        glBindVertexArray(mVAO);
        glDrawElements(GL_TRIANGLES, mIndexesCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(GL_NONE);
    }

    return lOk;
}


} // maz
