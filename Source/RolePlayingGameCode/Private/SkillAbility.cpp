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
    bReplicates = true; // Actor ��ü�� ���ø����̼��� Ȱ��ȭ
    SetReplicateMovement(true); // �̵� ���ø����̼� Ȱ��ȭ
    
    RootCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollisionComponent"));
    RootComponent = RootCollisionComponent;

    ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    ParticleSystemComponent->SetupAttachment(RootCollisionComponent);

    Event_Dele_DestroyAbility.AddDynamic(this, &ASkillAbility::InterruptCasting);
}

void ASkillAbility::BeginPlay()
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


void ASkillAbility::InitializeAbility(AIngameCharacter* InCaster, AActor* InTarget)
{
    if (HasAuthority())
    {
        Caster = InCaster;
        TargetToServer = InTarget;
    }

    AttachSelf();
    BeginCasting();
}

void ASkillAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASkillAbility, Caster);
    DOREPLIFETIME(ASkillAbility, TargetToServer);
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


void ASkillAbility::BeginCasting_Implementation()
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
        SuccessfulCast();
    }
}

void ASkillAbility::SuccessfulCast_Implementation()
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

