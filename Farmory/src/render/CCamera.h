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
    ECameraMode GetCameraMode() const;
    const TVec3& GetPosition() const;
    const TVec3& GetFront() const;
    const TVec3& GetUp() const;
    float GetNearPlaneDistance() const;
    float GetFarPlaneDistance() const;

    void SetCameraMode(ECameraMode aCameraMode);
    void SetPosition(const TVec3& aPosition);
    bool SetLookAt(const TVec3& aFront, const TVec3& aUp);
    bool SetClippingPlanes(float aNearDistance, float aFarDistance);

    // Perspective
    float GetFov() const;
    void SetFov(float aFov);

    float* GetViewMatrixPtr();
    float* GetProjMatrixPtr();

private:
    void RebuildViewMatrix();
    void RebuildProjMatrix(ECameraMode aMode);

    void OnFramebufferResized(int aWidth, int aHeight);

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
    float mWidth;
    float mHeight;
};

enum class ECameraMode : uint32
{
    ORTHOGRAPHIC,
    PERSPECTIVE
};

} // maz

#endif // !_H_CAMERA_
