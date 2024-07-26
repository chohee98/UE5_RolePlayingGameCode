#include "SkillAbility_Projectile.h"
#include "IngameCharacter.h"

ASkillAbility_Projectile::ASkillAbility_Projectile()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

    // Optionally set default values for the projectile movement component
    ProjectileMovementComponent->InitialSpeed = 800.f;
    ProjectileMovementComponent->MaxSpeed = 800.f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = false;
    ProjectileMovementComponent->bShouldBounce = false;
    ProjectileMovementComponent->bAutoActivate = false;
}

void ASkillAbility_Projectile::BeginPlay()
{
    Super::BeginPlay();

    /*if (SkillDetails.CastTime > 0.0f)
    {
        Caster->GetMesh();
    }
    else
    {

    }*/
}

void ASkillAbility_Projectile::ActivateEffect()
{
    Super::ActivateEffect();

    ProjectileMovementComponent->Activate(false);
}

void ASkillAbility_Projectile::DisplaySkill()
{
    Super::DisplaySkill();

    ActivateEffect();
}
