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

public:
	void AttachSelf();

	UFUNCTION(Client, Reliable)
	void BeginCasting();

	UFUNCTION(Client, Reliable)
	void SuccessfulCast();

	void InterruptCasting();

	virtual void ActivateEffect();
	virtual void DisplaySkill();

	UPROPERTY()
	FDele_DestroyAbility Event_Dele_DestroyAbility;


public:
	/*UPROPERTY(Replicated)
	ACharacter* Caster;

	UPROPERTY(Replicated)
	ATargetParent* Target;*/
	UPROPERTY(Replicated)
	class AIngameCharacter* Caster;

	UPROPERTY(Replicated)
	class AActor* TargetToServer;

	void InitializeAbility(AIngameCharacter* InCaster, AActor* InTarget);

	class UCastBarWidget* CastBarWidget;





public:
	class AIngameCharacter* PlayerRef;
	class AIngamePlayerController* Controller;
	class UUIMainWidget* MainWidget;
	class UCastBarWidget* CastbarWidget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FSkillStruct SkillDetails; // structure

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root")
	USphereComponent* RootCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystemComponent* ParticleSystemComponent;
};
