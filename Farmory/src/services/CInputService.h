#ifndef _H_C_INPUT_SERVICE_
#define _H_C_INPUT_SERVICE_

#include <services/IService.h>
#include <vector>


namespace maz
{
enum class EMouseAxis : uint32
{
    X_AXIS,
    Y_AXIS
};

class CInputService
    : public IService
{
public:
    CInputService();
    virtual ~CInputService();

    virtual bool Init();
    virtual void End();

    virtual bool IsOk() const;

    virtual void Update();

    // Information queries
    bool IsKeyDown(int aKey) const;
    float GetMousePosDelta(EMouseAxis aMouseAxis) const;
    float GetMouseScrollDelta(EMouseAxis aMouseAxis) const;

private:
    void OnKeyboardState(int aKey, int aScancode, int aAction, int aModifierBits);
    void OnMousePosition(double aXPos, double aYPos);
    void OnMouseScroll  (double aXOffset, double aYOffset);

private:
    // TMP Temporary members
    std::vector< int >mDownKeys;

    struct TMouseInfo
    {
        struct TAxisInfo
        {
            float           mLastPos;
            float           mNewPos;
            float           mPosOffset;
            float           mLastScrollOffset;
            float           mNewScrollOffset;

            TAxisInfo();
        };

        TAxisInfo mXAxis;
        TAxisInfo mYAxis;
    };

    void UpdateMouseAxis(TMouseInfo::TAxisInfo& aAxisInfo);

    TMouseInfo mMouseInfo;
};

} // maz

#endif // !_H_C_INPUT_SERVICE_
