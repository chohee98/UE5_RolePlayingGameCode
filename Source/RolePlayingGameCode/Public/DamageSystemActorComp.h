// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageSystemActorComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_TargetDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_OnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLEPLAYINGGAMECODE_API UDamageSystemActorComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageSystemActorComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	float Heal(float HealAmount);

	float TakeDamage(float DamageAmount);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDead;

public:
	// Event Dispatcher
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_TargetDamage Event_Dele_TargetDamaged;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_OnDeath Event_Dele_OnDeath;

};

