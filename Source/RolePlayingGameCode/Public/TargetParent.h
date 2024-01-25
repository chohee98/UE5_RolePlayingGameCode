// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageableInterface.h"
#include "Engine/SkeletalMesh.h"
#include "Components/PrimitiveComponent.h"
#include "DamageSystemActorComp.h"
#include "TargetParent.generated.h"

UCLASS()
class ROLEPLAYINGGAMECODE_API ATargetParent : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UDamageSystemActorComp* DamageSystem;

public:
	// Sets default values for this character's properties
	ATargetParent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	FString TargetName;



public:
	// 인터페이스 함수
	virtual float CurHp() override;
	virtual float MaxHp() override;
	virtual float GetHeal(float HealAmount) override;
	virtual float TakeDamage(float DamageAmount) override;

	// 컨스트럭션 스크립트
	UFUNCTION()
	virtual void OnConstruction(const FTransform& Transform) override;

	// 이벤트 디스패처
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TargetDeath();
	void TargetDeath_Implementation();

	UFUNCTION()
	void BeginCursorOver(UPrimitiveComponent* touchedComponent);

	UFUNCTION()
	void EndCursorOver(UPrimitiveComponent* touchedComponent);

	UFUNCTION()
	void SettingTarget(UPrimitiveComponent* touchedComponent, FKey ButtonPressed);


};
