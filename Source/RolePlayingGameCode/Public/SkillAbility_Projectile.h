#pragma once

#include "CoreMinimal.h"
#include "SkillAbility.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SkillAbility_Projectile.generated.h"

UCLASS()
class ROLEPLAYINGGAMECODE_API ASkillAbility_Projectile : public ASkillAbility
{
	GENERATED_BODY()

public:
    ASkillAbility_Projectile();

    virtual void ActivateEffect() override;
    virtual void DisplaySkill() override;

protected:
    virtual void BeginPlay() override;

public:
    // Projectile movement component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    UProjectileMovementComponent* ProjectileMovementComponent;

protected:
    void SetProjectileVelocityTowardsTarget();
	
};
