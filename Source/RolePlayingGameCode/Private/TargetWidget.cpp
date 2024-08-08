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
	pCharacter = pCharacter = Cast<AIngameCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	pCharacter->Event_Dele_TargetChanged.AddDynamic(this, &UTargetWidget::UpdateTargetUI);
	pCharacter->Event_Dele_OnTargetCancelled.AddDynamic(this, &UTargetWidget::WidgetCollapsed);
	pCharacter->Event_Dele_OnTargetDied.AddDynamic(this, &UTargetWidget::WidgetCollapsed);
}

void UTargetWidget::SetHpPersent()
{
	TargetHealthBar->SetPercent(pCharacter->CurrentTarget->CurHp() / pCharacter->CurrentTarget->MaxHp());
}

void UTargetWidget::UpdateTargetUI_Implementation()
{
	if (pCharacter && pCharacter->CurrentTarget)
	{
		// 기존 바인딩 제거
		pCharacter->CurrentTarget->DamageSystem->Event_Dele_TargetDamaged.RemoveDynamic(this, &UTargetWidget::UpdateTargetUI);

		// 새로운 타겟에 바인딩
		pCharacter->CurrentTarget->DamageSystem->Event_Dele_TargetDamaged.AddDynamic(this, &UTargetWidget::UpdateTargetUI);
		
		TargetNameText->SetText(FText::FromString(pCharacter->CurrentTarget->TargetName));
		SetHpPersent();
		this->SetVisibility(ESlateVisibility::Visible);
	}
	else
		this->SetVisibility(ESlateVisibility::Collapsed);	
}

void UTargetWidget::WidgetCollapsed_Implementation()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
