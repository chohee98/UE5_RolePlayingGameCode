#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageableInterface.h"
#include "Weapon.generated.h"

class AIngameCharacter;

UCLASS()
class ROLEPLAYINGGAMECODE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 콜리전 시작 이벤트 처리 함수
	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// AttackEffect 접근자 메서드
	UFUNCTION(BlueprintCallable, Category = "Effect")
	class UNiagaraComponent* GetAttackEffect() const { return AttackEffect; }

	// 캐릭터 소유자 설정 함수
	void SetOwningCharacter(AIngameCharacter* NewOwner);

	// 무기 콜리전 설정 함수
	void SetWeaponCollision(bool bEnable);

	// 오버랩 횟수 초기화 함수
	void ResetOverlapCount() { OverlapCount = 0; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* AttackEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	int32 OverlapCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DamageAmount = 50.0f; // 기본 데미지 값

	UPROPERTY()
	AIngameCharacter* OwningCharacter;

};
