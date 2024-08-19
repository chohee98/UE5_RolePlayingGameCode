#include "GameManager.h"
#include "Engine/World.h"

// ���� ��� ���� �ʱ�ȭ
UGameManager* UGameManager::Instance = nullptr;

UGameManager::UGameManager() : TimeOfDay(0.0f),CurrentWeather(EWeatherType::Clear)
{
}

UGameManager::~UGameManager()
{
}

UGameManager* UGameManager::Get()
{
    if (Instance == nullptr)
    {
        Instance = NewObject<UGameManager>();  // NewObject�� ����Ͽ� �ν��Ͻ� ����
        Instance->AddToRoot();  // ������ �÷��Ϳ� ���� �������� �ʵ��� �մϴ�.
    }
    return Instance;
}

float UGameManager::GetTimeOfDay() const
{
    return TimeOfDay;
}

UGameManager::EWeatherType UGameManager::GetWeather() const
{
    return CurrentWeather;
}

void UGameManager::SetRandomWeather()
{
    int32 RandomIndex = FMath::RandRange(0, static_cast<int32>(EWeatherType::Snow));
    CurrentWeather = static_cast<EWeatherType>(RandomIndex);
}

void UGameManager::StartGameTimeFlow(UWorld* World)
{  
    if (World)
        World->GetTimerManager().SetTimer(TimerHandle, this, &UGameManager::UpdateGameTime, 1.0f, true);
}

void UGameManager::UpdateGameTime()
{
    float GameTimePassed = 1.0f * RealTimeToGameTimeRatio;

    TimeOfDay += GameTimePassed / 3600.0f; // �ʸ� �ð� ������ ��ȯ
    if (TimeOfDay >= 24.0f)
        TimeOfDay -= 24.0f; // 24�ð��� �ʰ��ϸ� 0���� �ǵ��ư� 
}

