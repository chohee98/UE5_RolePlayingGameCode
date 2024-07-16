// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageableInterface.h"
#include "Engine/SkeletalMesh.h"
#include "Components/PrimitiveComponent.h"
#include "DamageSystemActorComp.h"
#include "TargetParent.generated.h"

#define ECC_Target ECC_GameTraceChannel1
#define ECC_Weapon ECC_GameTraceChannel2

UCLASS()
class ROLEPLAYINGGAMECODE_API ATargetParent : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ATargetParent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Damageable Interface 구현
	virtual float CurHp() override;
	virtual float MaxHp() override;
	virtual float GetHeal(float HealAmount) override;
	virtual float TakeDamage(float DamageAmount) override;

	// 이벤트 디스패처
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TargetDeath();
	virtual void TargetDeath_Implementation();

	// 컨스트럭션 스크립트
	UFUNCTION()
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void BeginCursorOver(UPrimitiveComponent* touchedComponent);

	UFUNCTION()
	void EndCursorOver(UPrimitiveComponent* touchedComponent);

	UFUNCTION()
	void SettingTarget(UPrimitiveComponent* touchedComponent, FKey ButtonPressed);

private:
	void SetupCollision(UPrimitiveComponent* Component);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UDamageSystemActorComp* DamageSystem;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	FString TargetName;

};
