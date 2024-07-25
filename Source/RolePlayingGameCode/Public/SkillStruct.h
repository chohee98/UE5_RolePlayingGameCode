#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillStruct.generated.h"

USTRUCT(BlueprintType)
struct FSkillStruct
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FName SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float ManaCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Cooldown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float CastTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    bool RequiresTarget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    UTexture2D* Icon;

    FSkillStruct()
        : SkillName(NAME_None)
        , Description(TEXT(""))
        , ManaCost(0.0f)
        , Cooldown(0.0f)
        , CastTime(0.0f)
        , RequiresTarget(true)
        , Icon(nullptr)
    {
    }
};