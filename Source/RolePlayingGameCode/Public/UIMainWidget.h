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
	void SetHpPersent();
	void SetMpPersent();

	UFUNCTION(BlueprintCallable, Category = "UI")
	UCastBarWidget* DisplayCastBar(class ASkillAbility* SkillAbility);

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

	UPROPERTY(meta = (BindWidget))
	class UCastBarWidget* CastBarWidget;

	// TargetWidget
};