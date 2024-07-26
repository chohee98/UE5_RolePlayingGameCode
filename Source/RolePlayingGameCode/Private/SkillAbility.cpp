#include "SkillAbility.h"
#include "IngameCharacter.h"
#include "IngamePlayerController.h"
#include "IngameHUD.h"
#include "UIMainWidget.h"
#include "CastBarWidget.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ASkillAbility::ASkillAbility()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true; // Actor 전체의 리플리케이션을 활성화

    RootCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollisionComponent"));
    RootComponent = RootCollisionComponent;

    ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    ParticleSystemComponent->SetupAttachment(RootCollisionComponent);
}

void ASkillAbility::BeginPlay()
{
    Super::BeginPlay();

    if (Caster)
    {
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
        AttachToComponent(Caster->GetMesh(), AttachmentRules, FName("SkillAttach"));
    }

    PlayerRef = Cast<AIngameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        Controller = Cast<AIngamePlayerController>(PlayerController);
        if (Controller)
        {
            AIngameHUD* HUD = Cast<AIngameHUD>(PlayerController->GetHUD());
            if (HUD)
                MainWidget = HUD->MainHUDWidget;
        }
    }

    // Start Casting the Skill
    BeginCasting();
}

void ASkillAbility::BeginCasting()
{
    if (1) // PlayerRef == Caster
    {
        if (PlayerRef->CheckMana(SkillDetails.ManaCost))
        {
            if (SkillDetails.CastTime > 0)
            {
                class UCastBarWidget* CastBarWidget = MainWidget->DisplayCastBar(this);
                if (CastBarWidget)
                    CastBarWidget->Event_Dele_CastSuccessful.AddDynamic(this, &ASkillAbility::DisplaySkill);
            }
            else
                DisplaySkill();               
        }
    }
}

void ASkillAbility::DisplaySkill()
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    PlayerRef->SpendMP(SkillDetails.ManaCost);
    SuccessfulCast();
}

void ASkillAbility::SuccessfulCast()
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("SuccessfulCast"));
}

void ASkillAbility::InterruptCasting()
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("InterruptCasting"));
    PlayerRef->DestroySkill(this);

}

void ASkillAbility::ActivateEffect()
{
    //DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

