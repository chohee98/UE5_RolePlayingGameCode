#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UIMainWidget.h"
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
	TSubclassOf<UUserWidget> MainHUDWidgetClass;
	UUIMainWidget* MainHUDWidget;
};
