#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "DamageNumberWidget.generated.h"

UCLASS()
class ROLEPLAYINGGAMECODE_API UDamageNumberWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Damage")
    void SetDamageText(float Damage);

    void PlayAnimationForward();

    void SetPosition(FVector2D ScreenPosition);

    void RemoveFromViewportWithDelay(float Delay);

    bool IsActive() const { return bIsActive; }
    void SetActive(bool Active) { bIsActive = Active; }

private:
    FTimerHandle RemoveTimerHandle;
    void RemoveFromViewport();

private:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DamageText;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* AnimationForward;

    bool bIsActive = false;
	
};
