// Fill out your copyright notice in the Description page of Project Settings.


#include "IngameHUD.h"

AIngameHUD::AIngameHUD()
{
	static ConstructorHelpers::FClassFinder<UUIMainWidget> MainHUDWidgetAsset(TEXT("WidgetBlueprint'/Game/RPG/Widget/InGame/MainCharacterWidget.MainCharacterWidget_C'"));
	//C:/Users/Tenth/Desktop/UE5_CH/RolePlayingGameCode/Content/RPG/Widget/InGame/MainCharacterWidget.uasset
	// TSubclassOf ���ø� Ŭ���� ��ü�� �������Ʈ Ŭ������ �־��ش�
	if (MainHUDWidgetAsset.Succeeded())
		MainHUDWidgetClass = MainHUDWidgetAsset.Class;
}

void AIngameHUD::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* pPC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	pPC->SetInputMode(FInputModeGameAndUI());
	pPC->SetShowMouseCursor(true);

	if (IsValid(MainHUDWidgetClass))
	{
		MainHUDWidget = Cast<UUIMainWidget>(CreateWidget(GetWorld(), MainHUDWidgetClass));

		if (IsValid(MainHUDWidget))
		{
			MainHUDWidget->AddToViewport();
		}
	}
}
