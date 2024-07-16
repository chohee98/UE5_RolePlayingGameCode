#include "CollisionNotify.h"
#include "IngameCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UCollisionNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp && MeshComp->GetOwner())
    {
        AIngameCharacter* Character = Cast<AIngameCharacter>(MeshComp->GetOwner());
        if (Character)
        {
            Character->CurrentWeapon->SetWeaponCollision(bEnableCollision);
        }
    }
}