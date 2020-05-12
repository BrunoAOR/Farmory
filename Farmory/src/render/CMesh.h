#ifndef _H_C_MESH_
#define _H_C_MESH_


namespace maz
{

class CMesh
{
public:
    CMesh();
    ~CMesh();

    bool InitRectangle(uint32 aWidth, uint32 aHeight);
    bool InitTriangle (uint32 aWidth, uint32 aHeight);
    bool InitCube     ();
    void End();

    bool IsOk() const;

    bool Draw() const;

private:
    uint32 mVAO;
    uint32 mVBO;
    uint32 mEBO;
    uint32 mIndexesCount;
};

} // maz

#endif // !_H_C_MESH_
