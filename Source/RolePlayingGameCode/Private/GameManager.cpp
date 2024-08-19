#include "GameManager.h"
#include "Engine/World.h"

// 정적 멤버 변수 초기화
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
        Instance = NewObject<UGameManager>();  // NewObject를 사용하여 인스턴스 생성
        Instance->AddToRoot();  // 가비지 컬렉터에 의해 삭제되지 않도록 합니다.
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

    TimeOfDay += GameTimePassed / 3600.0f; // 초를 시간 단위로 변환
    if (TimeOfDay >= 24.0f)
        TimeOfDay -= 24.0f; // 24시간을 초과하면 0으로 되돌아감 
}

