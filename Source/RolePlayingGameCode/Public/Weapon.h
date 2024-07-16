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

	// �ݸ��� ���� �̺�Ʈ ó�� �Լ�
	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// AttackEffect ������ �޼���
	UFUNCTION(BlueprintCallable, Category = "Effect")
	class UNiagaraComponent* GetAttackEffect() const { return AttackEffect; }

	// ĳ���� ������ ���� �Լ�
	void SetOwningCharacter(AIngameCharacter* NewOwner);

	// ���� �ݸ��� ���� �Լ�
	void SetWeaponCollision(bool bEnable);

	// ������ Ƚ�� �ʱ�ȭ �Լ�
	void ResetOverlapCount() { OverlapCount = 0; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* AttackEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	int32 OverlapCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DamageAmount = 50.0f; // �⺻ ������ ��

	UPROPERTY()
	AIngameCharacter* OwningCharacter;

};
