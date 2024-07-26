#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillStruct.h"
#include "SkillBarWidget.generated.h"

UCLASS()
class ROLEPLAYINGGAMECODE_API USkillBarWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Skill")
    void UpdateAppearance();

    void InitializeAppearance();

protected:
    virtual void SynchronizeProperties() override;

public:
    UPROPERTY(meta = (BindWidget))
    class UImage* ImageIcon;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* CoolDownBar;

    UPROPERTY(meta = (BindWidget))
    class UButton* CastButton;

    bool IsAvailable = true;

    

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (ExposeOnSpawn = "true"))
    TSubclassOf<class ASkillAbility> AbilityClass;

    UFUNCTION()
    void OnCastButtonClicked();
    
};
