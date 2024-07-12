// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetParent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "IngameCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATargetParent::ATargetParent() //: TargetName("허수아비")
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TargetMesh(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn'"));
	if (TargetMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TargetMesh.Object);
	}
	
	GetMesh()->OnBeginCursorOver.AddDynamic(this, &ATargetParent::BeginCursorOver);
	GetMesh()->OnEndCursorOver.AddDynamic(this, &ATargetParent::EndCursorOver);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	
	DamageSystem = CreateDefaultSubobject< UDamageSystemActorComp>(TEXT("DamageSystem"));
	this->AddOwnedComponent(DamageSystem);
}

// Called when the game starts or when spawned
void ATargetParent::BeginPlay()
{
	Super::BeginPlay();
	
	GetMesh()->OnClicked.AddDynamic(this, &ATargetParent::SettingTarget);
}


// Called to bind functionality to input
void ATargetParent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ATargetParent::CurHp()
{
	return DamageSystem->CurHp;
}

float ATargetParent::MaxHp()
{
	return DamageSystem->MaxHp;
}

float ATargetParent::GetHeal(float HealAmount)
{
	return DamageSystem->Heal(HealAmount);
}

float ATargetParent::TakeDamage(float DamageAmount)
{
	return DamageSystem->TakeDamage(DamageAmount);
}

void ATargetParent::OnConstruction(const FTransform& Transform)
{
	DamageSystem->MaxHp = 1500;
	DamageSystem->CurHp = DamageSystem->MaxHp;

	// Damage Component Actor's Event Dispathcher Ondeath event binding
	DamageSystem->Event_Dele_OnDeath.AddDynamic(this, &ATargetParent::TargetDeath);
}

void ATargetParent::TargetDeath_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Monster Death.."));
}


void ATargetParent::BeginCursorOver(UPrimitiveComponent* touchedComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Begin.."));
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	controller->CurrentMouseCursor = EMouseCursor::Hand;
	
}

void ATargetParent::EndCursorOver(UPrimitiveComponent* touchedComponent)
{
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	controller->CurrentMouseCursor = EMouseCursor::Default;
}

void ATargetParent::SettingTarget(UPrimitiveComponent* touchedComponent, FKey ButtonPressed)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Click.."));
	ACharacter* pPlayer0 = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AIngameCharacter* pCharacter = Cast<AIngameCharacter>(pPlayer0);
	pCharacter->SetTarget(this);
}


