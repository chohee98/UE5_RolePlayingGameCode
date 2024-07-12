// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IngamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ROLEPLAYINGGAMECODE_API AIngamePlayerController : public APlayerController
{
	GENERATED_BODY()

	AIngamePlayerController();
	
protected:
	virtual void BeginPlay() override;
};
