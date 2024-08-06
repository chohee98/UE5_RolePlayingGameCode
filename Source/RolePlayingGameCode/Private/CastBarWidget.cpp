#include "CastBarWidget.h"
#include "SkillAbility.h"
#include "TimerManager.h"
#include "IngameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCastBarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PlayerRef = Cast<AIngameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    PlayerRef->Event_Dele_InterruptCasting.AddDynamic(this, &UCastBarWidget::InterruptCast);
}

void UCastBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (IsVisible())
        UpdateCastBar();
}

void UCastBarWidget::StartCast()
{
    if (AbilityClass && PlayerRef)
    {
        ASkillAbility* DefaultAbility = Cast<ASkillAbility>(AbilityClass->GetDefaultObject());
        if (DefaultAbility)
        {
            CastFastRate = PlayerRef->GetCastfastRate() * DefaultAbility->SkillDetails.CastTime;
            GetWorld()->GetTimerManager().SetTimer(CastTimer, this, &UCastBarWidget::CompletedCast, CastFastRate, false);
        }       
    }
}

void UCastBarWidget::InterruptCast()
{
    while (IsVisible())
    {
        SetVisibility(ESlateVisibility::Collapsed);
        ClearCastTimer();

        PlayerRef->Server_DestroyAbility();
    } 
}

void UCastBarWidget::CompletedCast()
{   
    PlayerRef->Server_DisplaySkill();
    SetVisibility(ESlateVisibility::Collapsed);
}

void UCastBarWidget::UpdateCastBar()
{
    if (CastBar && TB_AbilityTime && GetWorld()->GetTimerManager().IsTimerActive(CastTimer))
    {
        float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CastTimer);
        float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(CastTimer);

        float Percent = FMath::Clamp(ElapsedTime / CastFastRate, 0.0f, 1.0f);
        CastBar->SetPercent(Percent);

        TB_AbilityTime->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), RemainingTime)));
    }
}

void UCastBarWidget::ClearCastTimer()
{
    if (GetWorld()->GetTimerManager().IsTimerActive(CastTimer))
        GetWorld()->GetTimerManager().ClearTimer(CastTimer);

    CastTimer.Invalidate();
}
