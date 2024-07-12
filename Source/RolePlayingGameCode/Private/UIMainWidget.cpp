// Fill out your copyright notice in the Description page of Project Settings.


#include "UIMainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "IngameCharacter.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#define LOCTEXT_NAMESPACE "CharacterState"

void UUIMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ACharacter* pPlayer0 = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AIngameCharacter* pCharacter = Cast<AIngameCharacter>(pPlayer0);
	pCharacter->Event_Dele_RequestUpdateUI.AddDynamic(this, &UUIMainWidget::UpdateBars);
	
}

void UUIMainWidget::SetHpPersent()
{
	ACharacter* pPlayer0 = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AIngameCharacter* pCharacter = Cast<AIngameCharacter>(pPlayer0);

	FText text = FText::Format(LOCTEXT("CharacterState", "HP({current} / {max}"), pCharacter->CurHp(), pCharacter->MaxHp());
	Text_HP->SetText(text);

	ProgressBar_HP->SetPercent(pCharacter->CurHp()/ pCharacter->MaxHp());
}

void UUIMainWidget::SetMpPersent()
{
	ACharacter* pPlayer0 = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AIngameCharacter* pCharacter = Cast<AIngameCharacter>(pPlayer0);

	FText text = FText::Format(LOCTEXT("CharacterState", "MP({current} / {max}"), pCharacter->GetCurrentMP(), pCharacter->GetMaxMP());
	Text_MP->SetText(text);

	ProgressBar_MP->SetPercent(pCharacter->GetCurrentMP() / pCharacter->GetMaxMP());
}

void UUIMainWidget::UpdateBars_Implementation()
{
	if (Text_HP)
		SetHpPersent();
	if (Text_MP)
		SetMpPersent();
}

#undef LOCTEXT_NAMESPACE

