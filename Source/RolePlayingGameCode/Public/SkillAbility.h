#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	void DisplayCastBar();
	void DisplaySkill();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	ACharacter* Caster;

public:
	class AIngameCharacter* PlayerRef;
	class AIngamePlayerController* Controller;
	class UUIMainWidget* MainWidget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FSkillStruct SkillDetails;


};
