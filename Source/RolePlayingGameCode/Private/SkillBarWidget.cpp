#include "SkillBarWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "SkillAbility.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "IngameCharacter.h"
#include "UObject/ConstructorHelpers.h"


void USkillBarWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    if (CastButton)
        CastButton->OnClicked.AddDynamic(this, &USkillBarWidget::OnCastButtonClicked);

    UpdateAppearance();
    InitializeAppearance();
}

void USkillBarWidget::OnCastButtonClicked()
{
    if (AbilityClass)
    {
        AIngameCharacter* PlayerCharacter = Cast<AIngameCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
        if (PlayerCharacter)
        {
            PlayerCharacter->ReqSpawnSkill(AbilityClass);
        }
    }
}

void USkillBarWidget::InitializeAppearance()
{
    if (AbilityClass)
    {
        ASkillAbility* DefaultAbility = Cast<ASkillAbility>(AbilityClass->GetDefaultObject());
        if (DefaultAbility)
        {
            if (DefaultAbility->SkillDetails.Icon)
            {
                ImageIcon->SetBrushFromTexture(DefaultAbility->SkillDetails.Icon);
                ImageIcon->SetVisibility(ESlateVisibility::Visible);
            }
            else
                ImageIcon->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    else
        ImageIcon->SetVisibility(ESlateVisibility::Collapsed);
}

void USkillBarWidget::UpdateAppearance()
{
    if (IsAvailable)
    {
        CoolDownBar->SetVisibility(ESlateVisibility::Collapsed);
        CoolDownBar->SetPercent(0.0f);
    }
    else
    {
        CoolDownBar->SetVisibility(ESlateVisibility::Visible);
        CoolDownBar->SetPercent(1.0f);
    }
}
