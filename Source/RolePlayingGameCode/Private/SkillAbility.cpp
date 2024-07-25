#include "SkillAbility.h"
#include "IngameCharacter.h"
#include "IngamePlayerController.h"
#include "IngameHUD.h"
#include "UIMainWidget.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ASkillAbility::ASkillAbility()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true; // Actor 전체의 리플리케이션을 활성화
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
    if (PlayerRef == Caster)
    {

        if (PlayerRef->CheckMana(50)) // 50 ManaCost를 임시로 넣어놓은 값
        {
            if (1)
            {
                DisplayCastBar();
                //CastSuccessful.Broadcast();
                
            }
            else
                DisplaySkill();
        }
    }
}

void ASkillAbility::DisplayCastBar()
{
}

void ASkillAbility::DisplaySkill()
{
}

