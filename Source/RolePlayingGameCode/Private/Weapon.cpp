// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "TargetParent.h"

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
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	// Niagara system load
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ParticleAsset(TEXT("NiagaraSystem'/Game/RPG/VFX/NS_SwordAttack.NS_SwordAttack'"));
	if (ParticleAsset.Succeeded())
	{
		AttackEffect->SetAsset(ParticleAsset.Object);
	}

	// 콜리전 이벤트 바인딩
	Weapon->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBeginOverlap);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();	
}

void AWeapon::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("11"));
	if (OtherActor && OtherActor != this)
	{
		ATargetParent* Target = Cast<ATargetParent>(OtherActor);
		if (Target)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Weapon hit the target!"));
			Target->TakeDamage(DamageAmount);
		}
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

