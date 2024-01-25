// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetWidget.h"
#include "Kismet/GameplayStatics.h"
#include "IngameCharacter.h"
#include "DamageSystemActorComp.h"
#include "TargetParent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"



void UTargetWidget::NativeConstruct()
{
	pPlayer0 = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AIngameCharacter* pCharacter = Cast<AIngameCharacter>(pPlayer0);
	pCharacter->Event_Dele_TargetChanged.AddDynamic(this, &UTargetWidget::UpdateRargetUI);
}

void UTargetWidget::SetHpPersent()
{
	AIngameCharacter* pCharacter = Cast<AIngameCharacter>(pPlayer0);

	TargetHealthBar->SetPercent(pCharacter->CurrentTarget->CurHp() / pCharacter->CurrentTarget->MaxHp());
}

void UTargetWidget::UpdateRargetUI_Implementation()
{
	AIngameCharacter* pCharacter = Cast<AIngameCharacter>(pPlayer0);
	if (pCharacter->CurrentTarget)
	{
		pCharacter->CurrentTarget->DamageSystem->Event_Dele_TargetDamaged.AddDynamic(this, &UTargetWidget::UpdateRargetUI);
		TargetNameText->SetText(FText::FromString(pCharacter->CurrentTarget->TargetName));
		SetHpPersent();
		this->SetVisibility(ESlateVisibility::Visible);
	}
	else
		this->SetVisibility(ESlateVisibility::Collapsed);
		
}
