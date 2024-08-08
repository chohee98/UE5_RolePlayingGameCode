#include "IngameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimInstance.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "IngameHUD.h"
#include "SkillAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AIngameCharacter::AIngameCharacter()
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
	GetCharacterMovement()->JumpZVelocity = 500.f;
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

	// Initialize zoom properties
	MinZoomDistance = 200.0f;
	MaxZoomDistance = 600.0f;
	ZoomSpeed = 50.0f;
	
	static ConstructorHelpers::FClassFinder<AWeapon> WeaponBPClass(TEXT("/Game/RPG/Character/MyWeapon"));
	if (WeaponBPClass.Class != NULL)
		MyWeapon = WeaponBPClass.Class;

	DamageSystem = CreateDefaultSubobject< UDamageSystemActorComp>(TEXT("DamageSystem"));
	this->AddOwnedComponent(DamageSystem);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SM(TEXT("AnimMontage'/Game/RPG/Character/Animations/SheathSword_T_Montage'"));
	if (SM.Succeeded())
		SheathMontage = SM.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DM(TEXT("AnimMontage'/Game/RPG/Character/Animations/DrawSword_T_Montage'"));
	if (DM.Succeeded())
		DrawMontage = DM.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BM_A(TEXT("AnimMontage'/Game/RPG/Character/Animations/BasicAttack_A_Montage'"));
	if (BM_A.Succeeded())
		BasicAttack_A = BM_A.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BM_B(TEXT("AnimMontage'/Game/RPG/Character/Animations/BasicAttack_B_Montage'"));
	if (BM_B.Succeeded())
		BasicAttack_B = BM_B.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DJ(TEXT("AnimMontage'/Game/RPG/Character/Animations/DoubleJump_Montage'"));
	if (DJ.Succeeded())
		DoubleJumpMontage = DJ.Object;

}

void AIngameCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	JumpCount = 0; // 캐릭터가 지면에 닿으면 점프 횟수를 초기화
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

		// InterruptCasting
		if (MovementVector.Y != 0.0f || MovementVector.X != 0.0f)
		{
			if (Event_Dele_InterruptCasting.IsBound())
				Event_Dele_InterruptCasting.Broadcast();
		}
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
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (Event_Dele_RequestUpdateUI.IsBound())	// RequestUpdateUI(Event Dispatcher) 호출
		Event_Dele_RequestUpdateUI.Broadcast();

	Event_Dele_InterruptCasting.AddDynamic(this, &AIngameCharacter::Server_StopAnim);
}

void AIngameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 리플리케이션 설정 추가
	DOREPLIFETIME(AIngameCharacter, CurrentTarget);

}

void AIngameCharacter::OnRep_CurrentTarget()
{
	if (CurrentTarget)
	{
		if (Event_Dele_TargetChanged.IsBound())
			Event_Dele_TargetChanged.Broadcast();
	}
	else
	{
		if (Event_Dele_OnTargetCancelled.IsBound())
			Event_Dele_OnTargetCancelled.Broadcast();
	}
}

void AIngameCharacter::EquipWeapon(const FInputActionValue& Value)  // input E
{
	Server_EquipWeapon();
}

void AIngameCharacter::BasicAttack(const FInputActionValue& Value)    // input Mouse
{
	if (GetCharacterMovement()->IsFalling())
	{
		return; // 점프 중이면 공격하지 않음
	}

	Server_Attack();
	//IDamageableInterface* Interface = Cast<IDamageableInterface>(GetCapsuleComponent()->GetOwner());
	//if (Interface != nullptr)
	//{
	//	Interface->TakeDamage(1000);
	//}
	//
	//if (Event_Dele_RequestUpdateUI.IsBound())	// RequestUpdateUI(Event Dispatcher) 호출
	//	Event_Dele_RequestUpdateUI.Broadcast();
}

void AIngameCharacter::TargetEsc(const FInputActionValue& Value)    // input Q
{
	if (CurrentTarget)
	{
		if (HasAuthority())
		{
			// 서버에서 타겟팅 취소
			CurrentTarget = nullptr;
			OnRep_CurrentTarget();
		}
		else
		{
			// 클라이언트에서 서버로 타겟팅 취소 요청
			Server_CancelTargeting();
		}
	}	
}

void AIngameCharacter::AttachWeapon()
{
	FName WeaponSocket(TEXT("WeaponHouse"));

	// MyWeapon 클래스를 사용하여 무기 인스턴스를 생성
	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(MyWeapon, FVector::ZeroVector, FRotator::ZeroRotator);
	if (CurrentWeapon)
	{
		FAttachmentTransformRules AttachmentRules(FAttachmentTransformRules::SnapToTargetIncludingScale);
		CurrentWeapon->AttachToComponent(GetMesh(), AttachmentRules, WeaponSocket);
		CurrentWeapon->SetOwningCharacter(this);  
	}
}

void AIngameCharacter::MoveWeaponToSocket(FName AttachSocketName)
{
	if (CurrentWeapon)
	{
		// 무기를 새로운 소켓에 부착
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		CurrentWeapon->AttachToComponent(GetMesh(), AttachmentRules, AttachSocketName);
	}
}

void AIngameCharacter::DrawWeapon()
{
	PlayAnimMontage(DrawMontage, 1.f);
	MoveWeaponToSocket("draw");
}

void AIngameCharacter::SheathWeapon()
{
	// 애니메이션 몽타주를 재생하고 끝났을 때 콜백을 등록
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AIngameCharacter::OnSheathMontageEnded);
		AnimInstance->Montage_Play(SheathMontage);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, SheathMontage);
	}
}

void AIngameCharacter::FlipFlopBasicAttackMontage()
{
	if (bIsMontagePlaying)
		return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 애니메이션 몽타주를 재생하고 끝났을 때 콜백을 등록	
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AIngameCharacter::OnBasicAttackhEnded);

		if (bBasicAttack)
		{
			AnimInstance->Montage_Play(BasicAttack_A);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, BasicAttack_A);
		}
		else
		{
			AnimInstance->Montage_Play(BasicAttack_B);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, BasicAttack_B);
		}
		bIsMontagePlaying = true; // 몽타주가 재생 중임을 표시
		bBasicAttack = !bBasicAttack;
	}
}

void AIngameCharacter::ActivateWeaponEffect(bool Active)
{
	if (CurrentWeapon && CurrentWeapon->GetAttackEffect())
	{
		if (Active)
			CurrentWeapon->GetAttackEffect()->Activate(true);
		else
			CurrentWeapon->GetAttackEffect()->Deactivate();
	}
}

void AIngameCharacter::OnSheathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == SheathMontage && !bInterrupted)
		MoveWeaponToSocket("WeaponHouse");
}

void AIngameCharacter::OnBasicAttackhEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Server_ShowDamage(CurrentWeapon->GetDamageAmout());
	ActivateWeaponEffect(false);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	bIsMontagePlaying = false; // 몽타주 재생 완료 표시
	CurrentWeapon->ResetOverlapCount();
}

void AIngameCharacter::DoubleJump()
{
	if (Event_Dele_InterruptCasting.IsBound())
		Event_Dele_InterruptCasting.Broadcast();

	Server_DoubleJump();
}

void AIngameCharacter::SpendMP(float ManaCost)
{
	NorMp = NorMp - ManaCost;
	CurMp = NorMp * MaxMp;
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

void AIngameCharacter::SetTarget(ATargetParent* Target)
{
	if (HasAuthority())
	{
		// 서버에서 바로 설정
		CurrentTarget = Target;
		OnRep_CurrentTarget();
	}
	else
	{
		// 클라이언트에서 서버로 RPC 호출
		Server_SetCurrentTarget(Target);
	}
}


void AIngameCharacter::CharacterDeath_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Death.."));
}

void AIngameCharacter::Server_EquipWeapon_Implementation()
{
	Multicast_EquipWeapon();
}

void AIngameCharacter::Multicast_EquipWeapon_Implementation()
{
	IsEquip = !IsEquip;

	if (IsEquip)
		DrawWeapon();
	else
		SheathWeapon();
}

void AIngameCharacter::Server_Attack_Implementation()
{
	Multicast_Attack();
}

void AIngameCharacter::Multicast_Attack_Implementation()
{
	if (IsEquip)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_None);
		ActivateWeaponEffect(true);
		FlipFlopBasicAttackMontage();
	}
}

void AIngameCharacter::Server_ShowDamage_Implementation(float DamageNum)
{
	Client_ShowDamage(DamageNum);
}

void AIngameCharacter::Client_ShowDamage_Implementation(float DamageNum)
{
	if (bTargetGetDamage == true)
	{
		//FString RoleString = HasAuthority() ? TEXT("Server") : TEXT("Client");
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, FString::Printf(TEXT("I'am [%s]"), *RoleString));
		// Get the player controller's HUD
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			AIngameHUD* HUD = Cast<AIngameHUD>(PlayerController->GetHUD());
			if (HUD)
				HUD->ShowDamageNumber(DamageNum, CurrentTarget->GetActorLocation());
		}
		bTargetGetDamage = false;
	}
	else
		return;
}

void AIngameCharacter::Server_DoubleJump_Implementation()
{
	Multicast_DoubleJump();
}

void AIngameCharacter::Multicast_DoubleJump_Implementation()
{
	if (JumpCount == 1)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && DoubleJumpMontage)
		{
			AnimInstance->Montage_Play(DoubleJumpMontage);
		}

		FVector CurrentVelocity = GetCharacterMovement()->Velocity;
		FVector LaunchVelocity = FVector(CurrentVelocity.X, CurrentVelocity.Y, 600.0f);

		// 이동 모드를 Falling으로 설정 (캐릭터가 공중에 있음을 알리고 중력 적용 및 공중 제어 가능)
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		LaunchCharacter(LaunchVelocity, true, true);

		JumpCount++;
	}
	else
	{
		ACharacter::Jump();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AIngameCharacter::DelayedFunction, 0.2f, false);
	}
}

void AIngameCharacter::Server_StopAnim_Implementation()
{
	Multicast_StopAnim();
}

void AIngameCharacter::Multicast_StopAnim_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		CurrentCastingMontage = AnimInstance->GetCurrentActiveMontage();
		if (CurrentCastingMontage && AnimInstance->Montage_IsPlaying(CurrentCastingMontage))
			AnimInstance->Montage_Stop(0.2f, CurrentCastingMontage);
	}
}

void AIngameCharacter::Server_SetCurrentTarget_Implementation(ATargetParent* NewTarget)
{
	if (NewTarget)
		CurrentTarget = NewTarget;
}

void AIngameCharacter::Server_CancelTargeting_Implementation()
{
	CurrentTarget = nullptr;
}


void AIngameCharacter::Server_SpawnAbility_Implementation(TSubclassOf<ASkillAbility> AbilityClass, ATargetParent* TargetToServer)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;

	FTransform SpawnTransform = GetActorTransform();
	SpawnedAbility = GetWorld()->SpawnActor<ASkillAbility>(AbilityClass, SpawnTransform, SpawnParams);

	if (SpawnedAbility)
		SpawnedAbility->InitializeAbility(this, TargetToServer);
}

void AIngameCharacter::Server_DestroyAbility_Implementation()
{
	if (IsValid(SpawnedAbility))
		SpawnedAbility->InterruptCasting();
}

void AIngameCharacter::Server_DisplaySkill_Implementation()
{
	if (SpawnedAbility)
	{
		// 액터 정보 출력
		/*FString AbilityInfo = FString::Printf(TEXT("Spawned Ability: %s"), *SpawnedAbility->GetName());
		AbilityInfo += FString::Printf(TEXT(", Class: %s"), *SpawnedAbility->GetClass()->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, AbilityInfo);*/

		SpawnedAbility->DisplaySkill();
	}
}
	
// Called to bind functionality to input
void AIngameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AIngameCharacter::DoubleJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AIngameCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AIngameCharacter::Look);

		//Weapon Ready
		EnhancedInputComponent->BindAction(ReadyAction, ETriggerEvent::Started, this, &AIngameCharacter::EquipWeapon);

		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AIngameCharacter::BasicAttack);

		//TargetEsc
		EnhancedInputComponent->BindAction(TargetEscAction, ETriggerEvent::Started, this, &AIngameCharacter::TargetEsc);
	}

	PlayerInputComponent->BindAxis("CameraZoom", this, &AIngameCharacter::ZoomCamera);
}

void AIngameCharacter::ZoomCamera(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		float NewArmLength = CameraBoom->TargetArmLength - (AxisValue * ZoomSpeed);
		NewArmLength = FMath::Clamp(NewArmLength, MinZoomDistance, MaxZoomDistance);
		CameraBoom->TargetArmLength = NewArmLength;
	}
}
