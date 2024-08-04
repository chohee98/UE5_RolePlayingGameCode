#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CastBarWidget.generated.h"

UCLASS()
class ROLEPLAYINGGAMECODE_API UCastBarWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability", meta = (ExposeOnSpawn = true))
    TSubclassOf<class ASkillAbility> AbilityClass;

    UFUNCTION(BlueprintCallable, Category = "Casting")
    void StartCast();

    UFUNCTION(BlueprintCallable, Category = "Casting")
    void InterruptCast();



private:
    void CompletedCast();
    void UpdateCastBar();
    void ClearCastTimer();


private:
    FTimerHandle CastTimer;
    float CastFastRate;
    class AIngameCharacter* PlayerRef;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* CastBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TB_AbilityTime;

};
