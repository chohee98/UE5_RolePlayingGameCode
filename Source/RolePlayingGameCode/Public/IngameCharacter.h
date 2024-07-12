// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DamageableInterface.h"
#include "DamageSystemActorComp.h"
#include "Weapon.h"
#include "TargetParent.h"
#include "NiagaraComponent.h"
#include "IngameCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_RequestUpdateUI);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_TargetChanged);

UCLASS()
class ROLEPLAYINGGAMECODE_API AIngameCharacter : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	
	/** InteractionT Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReadyAction;
	
	/** InteractionE Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

private:
	UPROPERTY(EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AWeapon> MyWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UDamageSystemActorComp* DamageSystem;

public:
	// Sets default values for this character's properties
	AIngameCharacter();

protected:
	/** Called for motion input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EquipWeapon(const FInputActionValue& Value);
	void BasicAttack(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


public:
	// 인터페이스 함수
	virtual float CurHp() override;
	virtual float MaxHp() override;
	virtual float GetHeal(float HealAmount) override;
	virtual float TakeDamage(float DamageAmount) override;
	
	// 컨스트럭션 스크립트
	UFUNCTION()
	virtual void OnConstruction(const FTransform& Transform) override;

	void SetTarget(ATargetParent* Target);

	// Event Dispatcher
	UPROPERTY()
	FDele_RequestUpdateUI Event_Dele_RequestUpdateUI;

	UPROPERTY()
	FDele_TargetChanged Event_Dele_TargetChanged;

	// 이벤트 디스패처 바인딩 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CharacterDeath();
	void CharacterDeath_Implementation();

	// Server & Client
	UFUNCTION(Server, Reliable)
	void ReqEquipWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void ResEquipWeapon();

	UFUNCTION(Server, Reliable)
	void ReqAttack();

	UFUNCTION(NetMulticast, Reliable)
	void ResAttack();

private:
	void AttachWeapon();
	void MoveWeaponToSocket(FName NewSocketName);
	
	void DrawWeapon();
	void SheathWeapon();
	void FlipFlopBasicAttackMontage();
	void ActivateWeaponEffect(bool Active);
	void OnSheathMontageEnded(UAnimMontage* Montage, bool bInterrupted); // 콜백 함수 선언
	void OnBasicAttackhEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	float GetMaxMP() { return MaxMp; }
	float GetCurrentMP() { return CurMp; }


public:
	// Target
	UPROPERTY(BlueprintReadOnly, Category = "Target")
	ATargetParent* CurrentTarget;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AWeapon* CurrentWeapon;

	

private:
	// Animation
	UAnimMontage* SheathMontage;
	UAnimMontage* DrawMontage;
	UAnimMontage* BasicAttack_A;
	UAnimMontage* BasicAttack_B;

	// 현재 무기 상태를 저장하는 변수
	bool IsEquip = false;

	// 일반 공격 모션 FlipFlop 결정 변수
	bool bBasicAttack = false;

	// Character MP Stats
	float MaxMp = 500;
	float CurMp = MaxMp;
	float NorMp;
};


