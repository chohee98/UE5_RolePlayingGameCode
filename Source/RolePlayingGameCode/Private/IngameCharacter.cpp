// Fill out your copyright notice in the Description page of Project Settings.


#include "IngameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AIngameCharacter::AIngameCharacter() : MaxMp(500), CurMp(MaxMp)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	DamageSystem = CreateDefaultSubobject< UDamageSystemActorComp>(TEXT("DamageSystem"));
	this->AddOwnedComponent(DamageSystem);

}

void AIngameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AIngameCharacter::Look(const FInputActionValue& Value)
{

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AIngameCharacter::Interaction(const FInputActionValue& Value)      // 임시로 인터페이스 확인용!! 
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Interaction"));	

	IDamageableInterface* Interface = Cast<IDamageableInterface>(GetCapsuleComponent()->GetOwner());
	if (Interface != nullptr)
	{
		Interface->TakeDamage(1000);
	}
	
	if (Event_Dele_RequestUpdateUI.IsBound())	// RequestUpdateUI(Event Dispatcher) 호출
		Event_Dele_RequestUpdateUI.Broadcast();
	
}

// Called when the game starts or when spawned
void AIngameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	AttachWeapon();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (Event_Dele_RequestUpdateUI.IsBound())	// RequestUpdateUI(Event Dispatcher) 호출
		Event_Dele_RequestUpdateUI.Broadcast();

}

float AIngameCharacter::CurHp()
{
	return DamageSystem->CurHp;
}

float AIngameCharacter::MaxHp()
{
	return DamageSystem->MaxHp;
}

float AIngameCharacter::GetHeal(float HealAmount)
{
	return DamageSystem->Heal(HealAmount);
}

float AIngameCharacter::TakeDamage(float DamageAmount)
{
	
	return DamageSystem->TakeDamage(DamageAmount);
}



void AIngameCharacter::OnConstruction(const FTransform& Transform)
{
	DamageSystem->MaxHp = 2500;
	DamageSystem->CurHp = DamageSystem->MaxHp;

	// Damage Component Actor's Event Dispathcher Ondeath event binding
	DamageSystem->Event_Dele_OnDeath.AddDynamic(this, &AIngameCharacter::CharacterDeath);
}

void AIngameCharacter::AttachWeapon()
{
	FName WeaponSocket(TEXT("WeaponHouse"));
	auto CurWeapon = GetWorld()->SpawnActor<AWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
}

void AIngameCharacter::SetTarget(ATargetParent* Target)
{
	CurrentTarget = Target;
	if (Event_Dele_TargetChanged.IsBound())	// TargetChanged (Event Dispatcher) 호출
		Event_Dele_TargetChanged.Broadcast();
}

void AIngameCharacter::CharacterDeath_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Death.."));
}
	

//// Called every frame
//void AIngameCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

// Called to bind functionality to input
void AIngameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AIngameCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AIngameCharacter::Look);

		//Interacting
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &AIngameCharacter::Interaction);

	}
}

