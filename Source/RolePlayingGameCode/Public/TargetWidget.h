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

	// ���ε� �Լ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateTargetUI();
	void UpdateTargetUI_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void WidgetCollapsed();
	void WidgetCollapsed_Implementation();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TargetNameText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* TargetHealthBar;

private:
	class AIngameCharacter* pCharacter = nullptr;
	
};
