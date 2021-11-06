#include "CTransformHierarchySystem.h"
#include <scenes/components/CTransform2DComponent.h>
#include <corecrt_math_defines.h>


namespace maz
{

namespace
{

TVec2 calculateWorldPosition(const STransform2D& arParentWorldTransform, const STransform2D& arLocalTransform)
{
    TVec2 lWorldPosition(0.0f, 0.0f);

    //	1. Solve the rotation
    //		1.1 Get polar coordinates for lLocalPosition (lR and lTheta)
    const TVec2& lLocalPosition = arLocalTransform.GetTranslation();
    const float lR = sqrt(lLocalPosition.x * lLocalPosition.x + lLocalPosition.y * lLocalPosition.y);
    const float lTheta = atan2(lLocalPosition.y, lLocalPosition.x);

    //		1.2 use the polar coordinate to recalculate the x and y coordinates
    const float lParentWorldRotation = arParentWorldTransform.GetRotation();
    lWorldPosition.x = lR * cosf(lTheta + (static_cast<float>(M_PI) / 180 * lParentWorldRotation));
    lWorldPosition.y = lR * sinf(lTheta + (static_cast<float>(M_PI) / 180 * lParentWorldRotation));

    //	2. Solve the scale (if 0, the effect of the rotation will be eliminated, but that's expected)
    const TVec2& lParentWorldScale = arParentWorldTransform.GetScale();
    lWorldPosition.x *= lParentWorldScale.x;
    lWorldPosition.y *= lParentWorldScale.y;

    //	3. Solve the position
    const TVec2& lParentWorldPosition = arParentWorldTransform.GetTranslation();
    lWorldPosition.x += lParentWorldPosition.x;
    lWorldPosition.y += lParentWorldPosition.y;

    return lWorldPosition;
}


float calculateWorldRotation(const float aParentWorldRotation, const float aLocalRotation)
{
    float lWorldRotation = aLocalRotation + aParentWorldRotation;
    // Clamp between 0 and 360
    // Note that rotations are always clamped between 0-360, so this algorythm works in the expected range.
    MAZ_ASSERT(aParentWorldRotation >= 0.0f && aParentWorldRotation < 360.0f, "aParentWorldRotation argument outside of expected range [0, 360[ !");
    MAZ_ASSERT(aLocalRotation >= 0.0f && aLocalRotation < 360.0f, "aLocalRotation argument outside of expected range [0, 360[ !");
    lWorldRotation -= 360 * (int)(lWorldRotation / 360);
    return (lWorldRotation);
}


TVec2 calculateWorldScale(const TVec2& aParentWorldScale, const TVec2& aLocalScale)
{
    // For scale, one only needs to multiply the parent scale
    TVec2 lWorldScale;
    lWorldScale.x = aLocalScale.x * aParentWorldScale.x;
    lWorldScale.y = aLocalScale.y * aParentWorldScale.y;
    return lWorldScale;
}

} // anonymous


CTransformHierarchySystem::CTransformHierarchySystem()
{
    AddComponentToSignature(EComponentType::Transform);
}


CTransformHierarchySystem::~CTransformHierarchySystem()
{
}


void CTransformHierarchySystem::Update()
{
    for (CReference<CGameObject>& lrGameObject : mGameObjects)
    {
        if (lrGameObject)
        {
            CReference<CTransform2DComponent> lHierarchyRootTransformComponent = lrGameObject->GetComponent<CTransform2DComponent>();

            // Update top-down full hierarchy only for the origin of any hierarchy

            if (!lHierarchyRootTransformComponent->GetParentTransform())
            {
                // Hierarchy root update

                // lFirstDirtyParentTransformComponent stores a reference to the first dirtyParent so that when going back up/sideways through the hierarchy, 
                //   we may know at which point we are back in "clean" territory 
                CReference<CTransform2DComponent> lFirstDirtyParentTransformComponent;

                if (lHierarchyRootTransformComponent->IsWorldTransformDirty())
                {
                    lFirstDirtyParentTransformComponent = lHierarchyRootTransformComponent;
                    lHierarchyRootTransformComponent->SetWorldTransform(lHierarchyRootTransformComponent->GetLocalTransform());
                    lHierarchyRootTransformComponent->RebuildModelMatrix();
                }

                // Update Children
                
                // We will navigate the hierarchy like this:
                // 1. Update the current transform
                // 2. Try to go down to FirstChild 
                // 2.1 If a FirstChild is available, go back to step 1.
                // 2.2 If NO FirstChild is available, go on to step 3.
                // 3. Try to go sideways to the NextSibling
                // 3.1 If a NextSibling is available, go back to step 1.
                // 3.2 If NO NextSibling is available, go on to step 4.
                // 4. Try to go up to Parent
                // 4.1 If a Parent is available (already updated), go back to step 3.
                // 4.2 If NO Parent is available, we're done.
                
                CReference<CTransform2DComponent> lCurrentChildTransformComponent = lHierarchyRootTransformComponent->GetFirstChildTransform();
                while (lCurrentChildTransformComponent)
                {
                    // Step 1
                    const bool lCurrentChildTransformDirty = lCurrentChildTransformComponent->IsWorldTransformDirty();
                    if (!lFirstDirtyParentTransformComponent && lCurrentChildTransformDirty)
                    {
                        lFirstDirtyParentTransformComponent = lCurrentChildTransformComponent;
                    }
                    
                    if (lFirstDirtyParentTransformComponent)
                    {
                        MAZ_ASSERT(lCurrentChildTransformComponent->GetParentTransform(), "Failed to find a Parent Transform, when one was expected!");
                        const STransform2D& lCurrentParentWorldTransform = lCurrentChildTransformComponent->GetParentTransform()->GetWorldTransform();
                        const STransform2D& lCurrentChildTransform = lCurrentChildTransformComponent->GetLocalTransform();
                        
                        STransform2D lNewChildWorldTransform;
                        lNewChildWorldTransform.SetTranslation(calculateWorldPosition(lCurrentParentWorldTransform, lCurrentChildTransform));
                        lNewChildWorldTransform.SetRotation(calculateWorldRotation(lCurrentParentWorldTransform.GetRotation(), lCurrentChildTransform.GetRotation()));
                        lNewChildWorldTransform.SetScale(calculateWorldScale(lCurrentParentWorldTransform.GetScale(), lCurrentChildTransform.GetScale()));

                        lCurrentChildTransformComponent->SetWorldTransform(lNewChildWorldTransform);
                        lCurrentChildTransformComponent->RebuildModelMatrix();
                    }

                    // Now we navigate
                    CReference<CTransform2DComponent> lCandidateTransform = lCurrentChildTransformComponent->GetFirstChildTransform(); // 2
                    if (lCandidateTransform)
                    {   // 2.1
                        lCurrentChildTransformComponent = lCandidateTransform;
                    }
                    else
                    {   // 2.2
                        // If we are moving sideways/up and the current Transform was the first dirty parent, then we are out of dirty territory
                        if (lFirstDirtyParentTransformComponent == lCurrentChildTransformComponent)
                        {
                            lFirstDirtyParentTransformComponent = CReference<CTransform2DComponent>();
                        }
                        lCandidateTransform = lCurrentChildTransformComponent->GetNextSiblingTransform(); // 3
                        if (lCandidateTransform)
                        {   // 3.1
                            lCurrentChildTransformComponent = lCandidateTransform;
                        }
                        else
                        {   // 3.2 We start going up
                            CReference<CTransform2DComponent> lParentTransform = lCurrentChildTransformComponent->GetParentTransform(); // 4
                            MAZ_ASSERT(lParentTransform, "First search for parent should always be successful!");
                            while (lParentTransform)
                            {   // 4.1
                                lCandidateTransform = lParentTransform->GetNextSiblingTransform(); // 3
                                if (lCandidateTransform)
                                {   // 3.1
                                    lCurrentChildTransformComponent = lCandidateTransform;
                                    lParentTransform = CReference<CTransform2DComponent>(); // This will exit the inner while-loop
                                }
                                else
                                {   // 3.2
                                    lParentTransform = lParentTransform->GetParentTransform(); // 4
                                }
                            }

                            if (!lCandidateTransform)
                            {   // 4.2
                                lCurrentChildTransformComponent = CReference<CTransform2DComponent>(); // This will exit the loop
                            }
                        }
                    }
                }
            }
        }
    }
}

} // maz
