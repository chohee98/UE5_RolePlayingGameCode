#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"

/**
 * GameManager 클래스 - 전역적으로 게임의 상태와 데이터를 관리하는 싱글톤 클래스
 */
class ROLEPLAYINGGAMECODE_API UGameManager : public UObject
{
public:
    enum class EWeatherType : uint8
    {
        Clear,
        Cloudy,
        Rain,
        Fog,
        Snow
    };

    // 싱글톤 인스턴스에 접근하기 위한 정적 메서드
    static UGameManager* Get();

    float GetTimeOfDay() const;
    EWeatherType GetWeather() const;

    void SetRandomWeather();
    void StartGameTimeFlow(UWorld* World);

private:
    // 싱글톤 인스턴스
    static UGameManager* Instance;

    // 생성자와 소멸자는 private으로 설정하여 외부에서 인스턴스를 생성하지 못하게 함
    UGameManager();
    ~UGameManager();

    // 복사 생성자와 대입 연산자도 비활성화하여 인스턴스 복사 방지!!
    UGameManager(const UGameManager&) = delete;
    UGameManager& operator=(const UGameManager&) = delete;

    FTimerHandle TimerHandle;
    void UpdateGameTime();

    float TimeOfDay;
    EWeatherType CurrentWeather;

    static inline const float RealTimeToGameTimeRatio = 6.0f;
    
};

