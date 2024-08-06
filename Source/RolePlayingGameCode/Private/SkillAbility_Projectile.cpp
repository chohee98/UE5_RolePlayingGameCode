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
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
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

void ASkillAbility_Projectile::SetProjectileVelocityTowardsTarget()
{
    if (HasAuthority())
    {
        if (Caster && Target)
        {
            FVector Direction = (Target->GetActorLocation() - Caster->GetActorLocation()).GetSafeNormal();
            FString TargetName = Target->GetName();
            FVector TargetLocation = Target->GetActorLocation();
            
            ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;

            FRotator TargetRotation = Direction.Rotation();
            SetActorRotation(TargetRotation);
        }
    }
}


void ASkillAbility_Projectile::ActivateEffect()
{
    Super::ActivateEffect();

    ProjectileMovementComponent->Activate(false);
}

void ASkillAbility_Projectile::DisplaySkill()
{
    SetProjectileVelocityTowardsTarget();

    Super::DisplaySkill();

    ActivateEffect();
}
