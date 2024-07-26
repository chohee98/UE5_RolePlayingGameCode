#include "SkillAbility_ProjectileArrow.h"

ASkillAbility_ProjectileArrow::ASkillAbility_ProjectileArrow()
{
    FSkillStruct NewSkillDetails;
    NewSkillDetails.SkillName = FName("Extreme Cold");
    NewSkillDetails.Description = FString("Shoots an ice arrow that deals damage to the target.");
    NewSkillDetails.ManaCost = 0.05f;
    NewSkillDetails.Cooldown = 10.0f;
    NewSkillDetails.CastTime = 3.0f;
    NewSkillDetails.RequiresTarget = true;
    // ������ �ε�
    static ConstructorHelpers::FObjectFinder<UTexture2D> Icon(TEXT("/Game/RPG/Widget/InGame/Textures/Arrow"));
    if (Icon.Succeeded())
        NewSkillDetails.Icon = Icon.Object;
    else
        NewSkillDetails.Icon = nullptr;

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/FXVarietyPack/Particles/P_ky_waterBall"));
    if (ParticleAsset.Succeeded())
        ParticleSystemComponent->SetTemplate(ParticleAsset.Object);

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