// Copyright Epic Games, Inc. All Rights Reserved.

#include "RolePlayingGameCodeGameMode.h"
#include "RolePlayingGameCodeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARolePlayingGameCodeGameMode::ARolePlayingGameCodeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter")); //C:/Users/Tenth/Desktop/UE5_CH/RolePlayingGameCode/Content/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.uasset
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
