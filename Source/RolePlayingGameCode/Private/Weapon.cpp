// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

//#include "Particles/ParticleSystemComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	//AttackEffect = CreateDefaultSubobject<UNiagaraSystemComponent>(TEXT("Effect"));

	RootComponent = Weapon;
	
	//AttackEffect->SetupAttachment(RootComponent);
	//AttackEffect->bAutoActivate = true;
	//AttackEffect->SetHiddenInGame(false);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("SkeletalMesh'/Game//RPG/Character/Assets/Weapon/Sword'"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	/*static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/RPG/VFX/NS_SwordAttack'"));
	if (ParticleAsset.Succeeded())
	{
		AttackEffect->SetTemplate(ParticleAsset.Object);
	}*/

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

