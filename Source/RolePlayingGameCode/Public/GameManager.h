#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"

/**
 * GameManager Ŭ���� - ���������� ������ ���¿� �����͸� �����ϴ� �̱��� Ŭ����
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

    // �̱��� �ν��Ͻ��� �����ϱ� ���� ���� �޼���
    static UGameManager* Get();

    float GetTimeOfDay() const;
    EWeatherType GetWeather() const;

    void SetRandomWeather();
    void StartGameTimeFlow(UWorld* World);

private:
    // �̱��� �ν��Ͻ�
    static UGameManager* Instance;

    // �����ڿ� �Ҹ��ڴ� private���� �����Ͽ� �ܺο��� �ν��Ͻ��� �������� ���ϰ� ��
    UGameManager();
    ~UGameManager();

    // ���� �����ڿ� ���� �����ڵ� ��Ȱ��ȭ�Ͽ� �ν��Ͻ� ���� ����!!
    UGameManager(const UGameManager&) = delete;
    UGameManager& operator=(const UGameManager&) = delete;

    FTimerHandle TimerHandle;
    void UpdateGameTime();

    float TimeOfDay;
    EWeatherType CurrentWeather;

    static inline const float RealTimeToGameTimeRatio = 6.0f;
    
};

