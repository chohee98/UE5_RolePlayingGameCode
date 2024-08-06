#include "SkillAbility.h"
#include "IngameCharacter.h"
#include "IngamePlayerController.h"
#include "IngameHUD.h"
#include "UIMainWidget.h"
#include "CastBarWidget.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASkillAbility::ASkillAbility()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true; // Actor 전체의 리플리케이션을 활성화
    SetReplicateMovement(true); // 이동 리플리케이션 활성화
    
    RootCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollisionComponent"));
    RootComponent = RootCollisionComponent;

    ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    ParticleSystemComponent->SetupAttachment(RootCollisionComponent);

    Event_Dele_DestroyAbility.AddDynamic(this, &ASkillAbility::InterruptCasting);
}

void ASkillAbility::BeginPlay() // 클라
{
    Super::BeginPlay();

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        PlayerRef = Cast<AIngameCharacter>(PlayerController->GetCharacter());
        Controller = Cast<AIngamePlayerController>(PlayerController);
        if (Controller)
        {
            AIngameHUD* HUD = Cast<AIngameHUD>(PlayerController->GetHUD());
            if (HUD)
                MainWidget = HUD->MainHUDWidget;
        }
    }
}


void ASkillAbility::InitializeAbility(AIngameCharacter* InCaster, ATargetParent* InTarget) // 서버
{
    if (HasAuthority())
    {
        Caster = InCaster;
        Target = InTarget;

        Multicast_RotateCharacterTowardsTarget(InCaster, InTarget);
    }

    AttachSelf();
    Client_BeginCasting();
}


void ASkillAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASkillAbility, Caster);
    DOREPLIFETIME(ASkillAbility, Target);
}

void ASkillAbility::AttachSelf()
{
    if (HasAuthority())
    {
        if (Caster)
        {
            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
            AttachToComponent(Caster->GetMesh(), AttachmentRules, FName("SkillAttach"));
        }
    }
}


void ASkillAbility::Client_BeginCasting_Implementation()
{
    if (PlayerRef->CheckMana(SkillDetails.ManaCost))
    {
        if (SkillDetails.CastTime > 0)
            CastbarWidget = MainWidget->DisplayCastBar(this);        
        else
            DisplaySkill();
    }
}

void ASkillAbility::DisplaySkill()
{
    if (HasAuthority())
    {
        DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Client_SuccessfulCast();
    }
}

void ASkillAbility::Client_SuccessfulCast_Implementation()
{
    PlayerRef->SpendMP(SkillDetails.ManaCost);   
}

void ASkillAbility::InterruptCasting()
{   
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    Destroy();  
}


void ASkillAbility::ActivateEffect()
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void ASkillAbility::AttackDamage()
{
    PlayerRef->Server_ShowDamage();
}

void ASkillAbility::Multicast_RotateCharacterTowardsTarget_Implementation(AIngameCharacter* RoCaster, ATargetParent* RoTarget)
{
    if (RoCaster && RoTarget)
    {
        FVector Direction = (RoTarget->GetActorLocation() - RoCaster->GetActorLocation()).GetSafeNormal();

        FRotator NewRotation = Direction.Rotation();
        RoCaster->SetActorRotation(NewRotation);
    }
}
