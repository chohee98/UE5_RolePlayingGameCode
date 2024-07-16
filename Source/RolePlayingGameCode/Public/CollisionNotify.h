#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CollisionNotify.generated.h"

UCLASS()
class ROLEPLAYINGGAMECODE_API UCollisionNotify : public UAnimNotify
{
    GENERATED_BODY()

public:
    // true�̸� �ݸ����� Ȱ��ȭ, false�̸� �ݸ����� ��Ȱ��ȭ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    bool bEnableCollision;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

