#include "SkillAbility_ProjectileArrow.h"
#include <Kismet/GameplayStatics.h>

ASkillAbility_ProjectileArrow::ASkillAbility_ProjectileArrow()
{
    FSkillStruct NewSkillDetails;
    NewSkillDetails.SkillName = FName("Extreme Cold");
    NewSkillDetails.Description = FString("Shoots an ice arrow that deals damage to the target.");
    NewSkillDetails.ManaCost = 0.05f;
    NewSkillDetails.Cooldown = 10.0f;
    NewSkillDetails.CastTime = 3.0f;
    NewSkillDetails.RequiresTarget = true;
    // 아이콘 로드
    static ConstructorHelpers::FObjectFinder<UTexture2D> Icon(TEXT("/Game/RPG/Widget/InGame/Textures/Arrow"));
    if (Icon.Succeeded())
        NewSkillDetails.Icon = Icon.Object;
    else
        NewSkillDetails.Icon = nullptr;

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/FXVarietyPack/Particles/P_ky_waterBall"));
    if (ParticleAsset.Succeeded())
        ParticleSystemComponent->SetTemplate(ParticleAsset.Object);

    static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticleAsset(TEXT("/Game/FXVarietyPack/Particles/P_ky_waterBallHit"));
    if (HitParticleAsset.Succeeded())
        HitEffect = HitParticleAsset.Object;

    SetSkillDetails(NewSkillDetails);  

    RootCollisionComponent->OnComponentHit.AddDynamic(this, &ASkillAbility_ProjectileArrow::OnHit);

    DamageAmount = 200.0f;
}

void ASkillAbility_ProjectileArrow::BeginPlay()
{
    Super::BeginPlay();
}

void ASkillAbility_ProjectileArrow::SetSkillDetails(const FSkillStruct& NewSkillDetails)
{
    SkillDetails = NewSkillDetails;
}

void ASkillAbility_ProjectileArrow::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        ATargetParent* HitTarget = Cast<ATargetParent>(OtherActor);
        if (HitTarget && HasAuthority()) // Only on server
            Multicast_OnHit(HitTarget, Hit);
        //AttackDamage();

        Destroy();
    }
}

void ASkillAbility_ProjectileArrow::Multicast_OnHit_Implementation(ATargetParent* OtherActor, const FHitResult& Hit)
{
    if (OtherActor)
    {
        if (HitEffect)
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint, FRotator::ZeroRotator, true);

        OtherActor->TakeDamage(DamageAmount);
    }
}
