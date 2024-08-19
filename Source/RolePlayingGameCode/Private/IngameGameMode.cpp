// Fill out your copyright notice in the Description page of Project Settings.
///Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter

#include "IngameGameMode.h"
#include "IngameHUD.h"
#include "IngamePlayerController.h"
#include "GameManager.h"

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

    PrimaryActorTick.bCanEverTick = true;
}

void AIngameGameMode::BeginPlay()
{
    Super::BeginPlay();

    UGameManager::Get()->SetRandomWeather();
    UGameManager::Get()->StartGameTimeFlow(GetWorld());
}

void AIngameGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // ���� �ΰ��� �ð��� ���� ��������
    float CurrentTime = UGameManager::Get()->GetTimeOfDay();
    UGameManager::EWeatherType CurrentWeather = UGameManager::Get()->GetWeather();

    // ������ ���ڿ��� ��ȯ
    FString WeatherString;
    switch (CurrentWeather)
    {
    case UGameManager::EWeatherType::Clear:
        WeatherString = "Clear";
        break;
    case UGameManager::EWeatherType::Cloudy:
        WeatherString = "Cloudy";
        break;
    case UGameManager::EWeatherType::Rain:
        WeatherString = "Rain";
        break;
    case UGameManager::EWeatherType::Fog:
        WeatherString = "Fog";
        break;
    case UGameManager::EWeatherType::Snow:
        WeatherString = "Snow";
        break;
    }

    // ��ũ���� �ð��� ������ ���
    if (GEngine)
    {
        FString FormattedTime = GetFormattedTime();
        GEngine->AddOnScreenDebugMessage(-1, 0.05f, FColor::Red, FString::Printf(TEXT("TimeOfDay: %s"), *FormattedTime));
        GEngine->AddOnScreenDebugMessage(-1, 0.05f, FColor::Cyan, FString::Printf(TEXT("Current Weather: %s"), *WeatherString));
    }
}

FString AIngameGameMode::GetFormattedTime() const
{
    float CurrentTime = UGameManager::Get()->GetTimeOfDay();
    // TimeOfDay�� �ð� ������ ����Ǿ� �����Ƿ�, �̸� ��, ��, �ʷ� ��ȯ
    int32 Hours = FMath::FloorToInt(CurrentTime);
    int32 Minutes = FMath::FloorToInt((CurrentTime - Hours) * 60.0f);
    int32 Seconds = FMath::FloorToInt(((CurrentTime - Hours) * 60.0f - Minutes) * 60.0f);

    return FString::Printf(TEXT("%02d:%02d:%02d"), Hours, Minutes, Seconds);
}