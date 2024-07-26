#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CastBarWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_CastSuccessful);

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

    // Event Dispatcher
    UPROPERTY(BlueprintAssignable, Category = "Casting")
    FDele_CastSuccessful Event_Dele_CastSuccessful;

private:
    void CompletedCast();
    void UpdateCastBar();


private:
    FTimerHandle CastTimer;
    float CastFastRate;
    class AIngameCharacter* PlayerRef;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* CastBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TB_AbilityTime;

};
