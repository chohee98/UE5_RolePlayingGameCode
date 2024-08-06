#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "SkillStruct.h"
#include "TargetParent.h"
#include "SkillAbility.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_DestroyAbility);

UCLASS()
class ROLEPLAYINGGAMECODE_API ASkillAbility : public AActor
{
	GENERATED_BODY()

public:
	ASkillAbility();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void ActivateEffect();	

	

public:
	UFUNCTION(Client, Reliable)
	void Client_BeginCasting();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RotateCharacterTowardsTarget(AIngameCharacter* RoCaster, ATargetParent* RoTarget);

	UFUNCTION(Client, Reliable)
	void Client_SuccessfulCast();

	void InterruptCasting();

	virtual void DisplaySkill();


public:
	UPROPERTY()
	FDele_DestroyAbility Event_Dele_DestroyAbility;

	UPROPERTY(Replicated)
	class AIngameCharacter* Caster;

	UPROPERTY(Replicated)
	class ATargetParent* Target;

	void InitializeAbility(AIngameCharacter* InCaster, ATargetParent* InTarget);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FSkillStruct SkillDetails; // structure

protected:
	void AttachSelf();
	void AttackDamage();
	void RotateCharacterTowardsTarget(AIngameCharacter* RoCaster, ATargetParent* RoTarget);

protected:
	class AIngameCharacter* PlayerRef;
	class AIngamePlayerController* Controller;
	class UUIMainWidget* MainWidget;
	class UCastBarWidget* CastbarWidget;

	USphereComponent* RootCollisionComponent;
	UParticleSystemComponent* ParticleSystemComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float DamageAmount = 0; // Damage amount for the skill
};
