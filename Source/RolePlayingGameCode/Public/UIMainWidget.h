// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIMainWidget.generated.h"

UCLASS()
class ROLEPLAYINGGAMECODE_API UUIMainWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetHpPersent();
	void SetMpPersent();

	// 바인딩 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateBars();
	void  UpdateBars_Implementation();


protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_HP;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ProgressBar_HP;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_MP;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ProgressBar_MP;

	// TargetWidget
};

