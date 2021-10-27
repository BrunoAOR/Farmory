#include "CTransformHierarchySystem.h"
#include <scenes/components/CTransform2DComponent.h>
#include <corecrt_math_defines.h>


namespace maz
{
TVec2 calculateWorldPosition(const STransform2D& aParentWorldTransform, const STransform2D& aLocalTransform)
{
    TVec2 worldPosition(0.0f, 0.0f);

    //	1. Solve the rotation
    //		1.1 Get polar coordinates for localPosition (r and theta)
    const TVec2& localPosition = aLocalTransform.GetTranslation();
    const float r = sqrt(localPosition.x * localPosition.x + localPosition.y * localPosition.y);
    const float theta = atan2(localPosition.y, localPosition.x);

    //		1.2 use the polar coordinate to recalculate the x and y coordinates
    const float parentWorldRotation = aParentWorldTransform.GetRotation();
    worldPosition.x = r * cosf(theta + (static_cast<float>(M_PI) / 180 * parentWorldRotation));
    worldPosition.y = r * sinf(theta + (static_cast<float>(M_PI) / 180 * parentWorldRotation));

    //	2. Solve the scale (if 0, the effect of the rotation will be eliminated, but that's expected)
    const TVec2& parentWorldScale = aParentWorldTransform.GetScale();
    worldPosition.x *= parentWorldScale.x;
    worldPosition.y *= parentWorldScale.y;

    //	3. Solve the position
    const TVec2& parentWorldPosition = aParentWorldTransform.GetTranslation();
    worldPosition.x += parentWorldPosition.x;
    worldPosition.y += parentWorldPosition.y;

    return worldPosition;
}


float calculateWorldRotation(const float aParentWorldRotation, const float aLocalRotation)
{
    float worldRotation = aLocalRotation + aParentWorldRotation;
    // Clamp between 0 and 360
    // Note that rotations are always clamped between 0-360, so this algorythm works in the expected range.
    MAZ_ASSERT(aParentWorldRotation >= 0.0f && aParentWorldRotation < 360.0f, "aParentWorldRotation argument outside of expected range [0, 360[ !");
    MAZ_ASSERT(aLocalRotation >= 0.0f && aLocalRotation < 360.0f, "aLocalRotation argument outside of expected range [0, 360[ !");
    worldRotation -= 360 * (int)(worldRotation / 360);
    return (worldRotation);
}


TVec2 calculateWorldScale(const TVec2& aParentWorldScale, const TVec2& aLocalScale)
{
    // For scale, one only needs to multiply the parent scale
    TVec2 worldScale;
    worldScale.x = aLocalScale.x * aParentWorldScale.x;
    worldScale.y = aLocalScale.y * aParentWorldScale.y;
    return worldScale;
}


CTransformHierarchySystem::CTransformHierarchySystem()
{
    AddComponentToSignature(EComponentType::Transform);
}


CTransformHierarchySystem::~CTransformHierarchySystem()
{
}


void CTransformHierarchySystem::Update()
{
    for (CReference<CGameObject>& gameObject : mGameObjects)
    {
        if (gameObject)
        {
            CReference<CTransform2DComponent> hierarchyRootTransformComponent = gameObject->GetComponent<CTransform2DComponent>();

            // Update top-down full hierarchy only for the origin of any hierarchy

            if (!hierarchyRootTransformComponent->GetParentTransform())
            {
                // Hierarchy root update

                // firstDirtyParentTransformComponent stores a reference to the first dirtyParent so that when going back up/sideways through the hierarchy, 
                //   we may know at which point we are back in "clean" territory 
                CReference<CTransform2DComponent> firstDirtyParentTransformComponent;

                if (hierarchyRootTransformComponent->IsWorldTransformDirty())
                {
                    firstDirtyParentTransformComponent = hierarchyRootTransformComponent;
                    hierarchyRootTransformComponent->SetWorldTransform(hierarchyRootTransformComponent->GetLocalTransform());
                    hierarchyRootTransformComponent->RebuildModelMatrix();
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
                
                CReference<CTransform2DComponent> currentChildTransformComponent = hierarchyRootTransformComponent->GetFirstChildTransform();
                while (currentChildTransformComponent)
                {
                    // Step 1
                    const bool currentChildTransformDirty = currentChildTransformComponent->IsWorldTransformDirty();
                    if (!firstDirtyParentTransformComponent && currentChildTransformDirty)
                    {
                        firstDirtyParentTransformComponent = currentChildTransformComponent;
                    }
                    
                    if (firstDirtyParentTransformComponent)
                    {
                        MAZ_ASSERT(currentChildTransformComponent->GetParentTransform(), "Failed to find a Parent Transform, when one was expected!");
                        const STransform2D& currentParentWorldTransform = currentChildTransformComponent->GetParentTransform()->GetWorldTransform();
                        const STransform2D& currentChildTransform = currentChildTransformComponent->GetLocalTransform();
                        
                        STransform2D newChildWorldTransform;
                        newChildWorldTransform.SetTranslation(calculateWorldPosition(currentParentWorldTransform, currentChildTransform));
                        newChildWorldTransform.SetRotation(calculateWorldRotation(currentParentWorldTransform.GetRotation(), currentChildTransform.GetRotation()));
                        newChildWorldTransform.SetScale(calculateWorldScale(currentParentWorldTransform.GetScale(), currentChildTransform.GetScale()));

                        currentChildTransformComponent->SetWorldTransform(newChildWorldTransform);
                        currentChildTransformComponent->RebuildModelMatrix();
                    }

                    // Now we navigate
                    CReference<CTransform2DComponent> candidateTransform = currentChildTransformComponent->GetFirstChildTransform(); // 2
                    if (candidateTransform)
                    {   // 2.1
                        currentChildTransformComponent = candidateTransform;
                    }
                    else
                    {   // 2.2
                        // If we are moving sideways/up and the current Transform was the first dirty parent, then we are out of dirty territory
                        if (firstDirtyParentTransformComponent == currentChildTransformComponent)
                        {
                            firstDirtyParentTransformComponent = CReference<CTransform2DComponent>();
                        }
                        candidateTransform = currentChildTransformComponent->GetNextSiblingTransform(); // 3
                        if (candidateTransform)
                        {   // 3.1
                            currentChildTransformComponent = candidateTransform;
                        }
                        else
                        {   // 3.2 We start going up
                            CReference<CTransform2DComponent> parentTransform = currentChildTransformComponent->GetParentTransform(); // 4
                            MAZ_ASSERT(parentTransform, "First search for parent should always be successful!");
                            while (parentTransform)
                            {   // 4.1
                                candidateTransform = parentTransform->GetNextSiblingTransform(); // 3
                                if (candidateTransform)
                                {   // 3.1
                                    currentChildTransformComponent = candidateTransform;
                                    parentTransform = CReference<CTransform2DComponent>(); // This will exit the inner while-loop
                                }
                                else
                                {   // 3.2
                                    parentTransform = parentTransform->GetParentTransform(); // 4
                                }
                            }

                            if (!candidateTransform)
                            {   // 4.2
                                currentChildTransformComponent = CReference<CTransform2DComponent>(); // This will exit the loop
                            }
                        }
                    }
                }
            }
        }
    }
}

} // maz
