#ifndef _H_CAMERA_
#define _H_CAMERA_

namespace maz
{
enum class ECameraMode : uint32;

class CCamera
{
public:
    CCamera();
    ~CCamera();

    bool Init();
    void End();

    // General
    void SetCameraMode(ECameraMode aCameraMode);
    void SetPosition(const TVec3& aPosition);
    bool SetLookAt(const TVec3& aFront, const TVec3& aUp);
    bool SetClippingPlanes(float aNearDistance, float aFarDistance);

    // Perspective
    void SetFov(float aFov);

    const float* GetViewMatrixPtr() const;
    const float* GetProjMatrixPtr() const;

private:
    void RebuildViewMatrix();
    void RebuildProjMatrix(ECameraMode aMode);

    void OnWindowResized(int aWidth, int aHeight);

private:
    ECameraMode mMode;
    TVec3   mPos;
    TVec3   mFront;
    TVec3   mUp;
    TMat4x4 mViewMatrix;
    TMat4x4 mProjMatrix;
    float mNear;
    float mFar;
    float mFov;
    int   mWidth;
    int   mHeight;
};

enum class ECameraMode : uint32
{
    ORTHOGRAPHIC,
    PERSPECTIVE
};

} // maz

#endif // !_H_CAMERA_
