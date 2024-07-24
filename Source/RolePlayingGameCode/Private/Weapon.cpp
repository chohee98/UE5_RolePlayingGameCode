// Fill out your copyright notice in the Description page of Project Settings.
#include "Weapon.h"
#include "IngameCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "TargetParent.h"
#include "Engine/Engine.h"
#include "DamageNumberWidget.h"
#include "IngameHUD.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Weapon mesh component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	// Niagara system component
	AttackEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	AttackEffect->SetupAttachment(RootComponent);
	AttackEffect->bAutoActivate = false;
	AttackEffect->SetHiddenInGame(false);

	// Weapon mesh load
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("SkeletalMesh'/Game//RPG/Character/Assets/Weapon/Sword'"));
	if (SK_WEAPON.Succeeded())
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);

	// Set collision profile name to "Weapon"
	Weapon->SetCollisionProfileName(TEXT("Weapon"));

	// Niagara system load
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ParticleAsset(TEXT("NiagaraSystem'/Game/RPG/VFX/NS_SwordAttack.NS_SwordAttack'"));
	if (ParticleAsset.Succeeded())
		AttackEffect->SetAsset(ParticleAsset.Object);

	// 중복 바인딩 방지
	Weapon->OnComponentBeginOverlap.RemoveDynamic(this, &AWeapon::OnWeaponBeginOverlap);
	// Bind the overlap event
	Weapon->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBeginOverlap);

}
// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Set Collision
	SetWeaponCollision(false);
}

void AWeapon::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OverlapCount < 1)
	{
		ATargetParent* Target = Cast<ATargetParent>(OtherActor);

		if (Target)
		{
			if (OwningCharacter)
				OwningCharacter->SetTargetGetDamage();  // AIngameCharacter의 함수 호출	

			Target->TakeDamage(DamageAmount);
			OverlapCount++;			
		}
	}
}

void AWeapon::SetWeaponCollision(bool bEnable)
{
	Weapon->SetGenerateOverlapEvents(bEnable);

	if (bEnable)
		Weapon->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	else
		Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::SetOwningCharacter(AIngameCharacter* NewOwner)
{
	OwningCharacter = NewOwner;
}
