#ifndef _H_I_COMPONENT_
#define _H_I_COMPONENT_

#include<maz/CReference.h>


namespace maz
{

class CGameObject;
class IComponent
{
public:
    IComponent(CReference<CGameObject>& aOwner);
    ~IComponent();

    const CReference<CGameObject> GetOwner() const;
    CReference<CGameObject> GetOwner();

private:
    CReference<CGameObject> mOwner;
};

} // maz

#endif // !_H_I_COMPONENT_
