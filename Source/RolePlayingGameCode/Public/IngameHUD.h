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
};
