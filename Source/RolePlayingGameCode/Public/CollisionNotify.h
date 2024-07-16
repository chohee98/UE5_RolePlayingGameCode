#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CollisionNotify.generated.h"

UCLASS()
class ROLEPLAYINGGAMECODE_API UCollisionNotify : public UAnimNotify
{
    GENERATED_BODY()

public:
    // true이면 콜리전을 활성화, false이면 콜리전을 비활성화
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    bool bEnableCollision;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

