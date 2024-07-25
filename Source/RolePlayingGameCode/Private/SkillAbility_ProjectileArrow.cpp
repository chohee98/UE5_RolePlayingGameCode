#include "SkillAbility_ProjectileArrow.h"

ASkillAbility_ProjectileArrow::ASkillAbility_ProjectileArrow()
{
    FSkillStruct NewSkillDetails;
    NewSkillDetails.SkillName = FName("Extreme Cold");
    NewSkillDetails.Description = FString("Shoots an ice arrow that deals damage to the target.");
    NewSkillDetails.ManaCost = 0.05f;
    NewSkillDetails.Cooldown = 2.0f;
    NewSkillDetails.CastTime = 0.0f;
    NewSkillDetails.RequiresTarget = true;
    // 아이콘 로드
    static ConstructorHelpers::FObjectFinder<UTexture2D> Icon(TEXT("/Game/RPG/Widget/InGame/Textures/Arrow"));
    if (Icon.Succeeded())
        NewSkillDetails.Icon = Icon.Object;
    else
        NewSkillDetails.Icon = nullptr;

    SetSkillDetails(NewSkillDetails);  
}

void ASkillAbility_ProjectileArrow::BeginPlay()
{
    Super::BeginPlay();
}

void ASkillAbility_ProjectileArrow::SetSkillDetails(const FSkillStruct& NewSkillDetails)
{
    SkillDetails = NewSkillDetails;
}