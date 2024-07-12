// Fill out your copyright notice in the Description page of Project Settings.
///Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter

#include "IngameGameMode.h"
#include "IngameHUD.h"
#include "IngamePlayerController.h"

AIngameGameMode::AIngameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/RPG/Character/BP_IngameCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	HUDClass = AIngameHUD::StaticClass();
	PlayerControllerClass = AIngamePlayerController::StaticClass();
}
