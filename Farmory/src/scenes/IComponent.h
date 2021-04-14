#ifndef _H_I_COMPONENT_
#define _H_I_COMPONENT_


namespace maz
{

class CGameObject;
class IComponent
{
public:
    IComponent(CGameObject& aOwner);

    const CGameObject& GetOwner() const;
    CGameObject& GetOwner();

private:
    CGameObject& mOwner;
};

} // maz

#endif // !_H_I_COMPONENT_
