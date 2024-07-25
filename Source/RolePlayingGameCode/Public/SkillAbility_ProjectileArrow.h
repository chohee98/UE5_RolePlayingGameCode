#pragma once

#include "CoreMinimal.h"
#include "SkillAbility_Projectile.h"
#include "SkillAbility_ProjectileArrow.generated.h"

UCLASS()
class ROLEPLAYINGGAMECODE_API ASkillAbility_ProjectileArrow : public ASkillAbility_Projectile
{
	GENERATED_BODY()

public:
    ASkillAbility_ProjectileArrow();

protected:
    virtual void BeginPlay() override;

public:
    // SkillDetails ����ü ��� �Լ�
    void SetSkillDetails(const FSkillStruct& NewSkillDetails);
	
};
