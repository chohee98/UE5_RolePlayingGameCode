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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_OnTargetCancelled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_InterruptCasting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_OnTargetDied);


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
	
	/** InteractionE Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReadyAction;
	
	/** InteractionMouse Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	/** InteractionQ Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TargetEscAction;

private:
	UPROPERTY(EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AWeapon> MyWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UDamageSystemActorComp* DamageSystem;

public:
	// Sets default values for this character's properties
	AIngameCharacter();

	//void DoubleJump();
	void ResetJumpCount() { JumpCount = 0; };

protected:
	/** Called for motion input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EquipWeapon(const FInputActionValue& Value);
	void BasicAttack(const FInputActionValue& Value);
	void TargetEsc(const FInputActionValue& Value);

	virtual void Landed(const FHitResult& Hit) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

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

	UPROPERTY()
	FDele_OnTargetCancelled Event_Dele_OnTargetCancelled;

	UPROPERTY()
	FDele_InterruptCasting Event_Dele_InterruptCasting;

	UPROPERTY()
	FDele_OnTargetDied Event_Dele_OnTargetDied;


	// 이벤트 디스패처 바인딩 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CharacterDeath();
	void CharacterDeath_Implementation();

	// Server & Client
	UFUNCTION(Server, Reliable)
	void Server_EquipWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EquipWeapon();

	UFUNCTION(Server, Reliable)
	void Server_Attack();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Attack();

	UFUNCTION(Server, Reliable)
	void Server_ShowDamage(float DamageNum);

	UFUNCTION(Client, Reliable)
	void Client_ShowDamage(float DamageNum);

	UFUNCTION(Server, Reliable)
	void Server_DoubleJump();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DoubleJump();

	UFUNCTION(Server, Reliable)
	void Server_StopAnim();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopAnim();

	UFUNCTION(Server, Reliable)
	void Server_SpawnAbility(TSubclassOf<ASkillAbility> AbilityClass, ATargetParent* TargetToServer);

	UFUNCTION(Server, Reliable)
	void Server_DestroyAbility();

	UFUNCTION(Server, Reliable)
	void Server_DisplaySkill();

	UFUNCTION(Server, Reliable)
	void Server_SetCurrentTarget(ATargetParent* NewTarget);

	UFUNCTION(Server, Reliable)
	void Server_CancelTargeting();

private:
	void ZoomCamera(float AxisValue);
	void AttachWeapon();
	void MoveWeaponToSocket(FName NewSocketName);
	
	void DrawWeapon();
	void SheathWeapon();
	void FlipFlopBasicAttackMontage();
	void ActivateWeaponEffect(bool Active);
	void OnSheathMontageEnded(UAnimMontage* Montage, bool bInterrupted); // 콜백 함수 선언
	void OnBasicAttackhEnded(UAnimMontage* Montage, bool bInterrupted);

	void DoubleJump();

	FTimerHandle TimerHandle;
	void DelayedFunction() { JumpCount++; };

public:
	float GetMaxMP() { return MaxMp; }
	float GetCurrentMP() { return CurMp; }
	float GetCastfastRate() { return CastfastRate; }
	void  SetTargetGetDamage() { bTargetGetDamage = true; };
	bool  CheckMana(float ManaCost) { return ManaCost <= NorMp; }
	void  SpendMP(float ManaCost);

public:
	// Target
	UPROPERTY(ReplicatedUsing = OnRep_CurrentTarget)
	ATargetParent* CurrentTarget;

	UFUNCTION()
	void OnRep_CurrentTarget();

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AWeapon* CurrentWeapon;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Target", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	//ATargetParent* TargetToServer;

	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	ASkillAbility* SpawnedAbility;

private:
	// Animation
	UAnimMontage* SheathMontage;
	UAnimMontage* DrawMontage;
	UAnimMontage* BasicAttack_A;
	UAnimMontage* BasicAttack_B;
	UAnimMontage* DoubleJumpMontage;

	// Zoom settings
	float MinZoomDistance;
	float MaxZoomDistance;
	float ZoomSpeed;

	// 현재 무기 상태를 저장하는 변수
	bool IsEquip = false;

	// 일반 공격 모션 FlipFlop 결정 변수
	bool bBasicAttack = false;

	// 애니메이션 몽타주 재생 여부를 추적할 변수
	bool bIsMontagePlaying = false;

	// 데미지 여부를 추적할 변수
	bool bTargetGetDamage = false;

	// Character MP Stats
	float MaxMp = 500;
	float CurMp = MaxMp;
	float NorMp = 1.0f;

	// Jump Twice
	int32 JumpCount = 0;
	int32 MaxJumpCount = 2;

	// 시전 속도
	float CastfastRate = 1.0f;

	// 캐스팅 몽타주를 저장할 변수
	UAnimMontage* CurrentCastingMontage;

};


