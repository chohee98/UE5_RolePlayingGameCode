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

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowDamageNumber(float Damage, FVector Location);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MainHUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UDamageNumberWidget> DamageNumberWidgetClass;

	// �� �̻� ������ �ʴ� ������ �����ϴ� �Լ�
	void CleanupDamageWidgets();
	UDamageNumberWidget* GetPooledDamageWidget();

public:
	UUIMainWidget* MainHUDWidget;
	TQueue<UDamageNumberWidget*> WidgetQueue;

private:
	// Ȱ��ȭ�� ������ ������ �����ϴ� �迭
	TArray<UDamageNumberWidget*> ActiveDamageWidgets;

	FTimerHandle CleanupTimerHandle; // Ÿ�̸� �ڵ�

	
};
