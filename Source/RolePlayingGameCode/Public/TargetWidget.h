// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetWidget.generated.h"

UCLASS()
class ROLEPLAYINGGAMECODE_API UTargetWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetHpPersent();

	// 바인딩 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateRargetUI();
	void UpdateRargetUI_Implementation();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TargetNameText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* TargetHealthBar;

	ACharacter* pPlayer0 = nullptr;


	
};
