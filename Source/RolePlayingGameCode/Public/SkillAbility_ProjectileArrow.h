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

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_OnHit(ATargetParent* OtherActor, const FHitResult& Hit);

private:
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    UParticleSystem* HitEffect;

};
