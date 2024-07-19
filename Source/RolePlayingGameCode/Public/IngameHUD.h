#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UIMainWidget.h"
#include "DamageNumberWidget.h"
#include "IngameHUD.generated.h"

UCLASS()
class ROLEPLAYINGGAMECODE_API AIngameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AIngameHUD();

protected:
	virtual void BeginPlay() override;	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MainHUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UDamageNumberWidget> DamageNumberWidgetClass;

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowDamageNumber(float Damage, FVector Location);

	UUIMainWidget* MainHUDWidget;

private:
	// 활성화된 데미지 위젯을 관리하는 배열
	TArray<UDamageNumberWidget*> ActiveDamageWidgets;

	// 더 이상 사용되지 않는 위젯을 정리하는 함수
	void CleanupDamageWidgets();
};
