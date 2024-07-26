#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "SkillStruct.h"
#include "SkillAbility.generated.h"

UCLASS()
class ROLEPLAYINGGAMECODE_API ASkillAbility : public AActor
{
	GENERATED_BODY()

public:
	ASkillAbility();

protected:
	virtual void BeginPlay() override;


public:
	void BeginCasting();
	void SuccessfulCast();
	void InterruptCasting();

	virtual void ActivateEffect();
	virtual void DisplaySkill();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	ACharacter* Caster;

public:
	class AIngameCharacter* PlayerRef;
	class AIngamePlayerController* Controller;
	class UUIMainWidget* MainWidget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FSkillStruct SkillDetails; // structure

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root")
	USphereComponent* RootCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystemComponent* ParticleSystemComponent;
};
