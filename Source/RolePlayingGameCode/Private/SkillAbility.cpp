#include "SkillAbility.h"
#include "IngameCharacter.h"
#include "IngamePlayerController.h"
#include "IngameHUD.h"
#include "UIMainWidget.h"
#include "CastBarWidget.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASkillAbility::ASkillAbility()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true; // Actor 전체의 리플리케이션을 활성화

    RootCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollisionComponent"));
    RootComponent = RootCollisionComponent;

    ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    ParticleSystemComponent->SetupAttachment(RootCollisionComponent);

    Event_Dele_DestroyAbility.AddDynamic(this, &ASkillAbility::InterruptCasting);
}

void ASkillAbility::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        PlayerRef = Cast<AIngameCharacter>(PlayerController->GetCharacter());
        Controller = Cast<AIngamePlayerController>(PlayerController);
        if (Controller)
        {
            AIngameHUD* HUD = Cast<AIngameHUD>(PlayerController->GetHUD());
            if (HUD)
                MainWidget = HUD->MainHUDWidget;
        }
    }
}


void ASkillAbility::InitializeAbility(AIngameCharacter* InCaster, AActor* InTarget)
{
    if (HasAuthority())
    {
        Caster = InCaster;
        TargetToServer = InTarget;


        // Caster가 제대로 설정되었는지 디버그 메시지로 확인
        FString CasterName = Caster->GetName();
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Caster in Multicast: %s"), *CasterName));
    }

    AttachSelf();
    BeginCasting();
}

void ASkillAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASkillAbility, Caster);
    DOREPLIFETIME(ASkillAbility, TargetToServer);
}

void ASkillAbility::AttachSelf()
{
    if (HasAuthority()) // 서버에서만 실행
    {
        if (Caster)
        {
            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
            AttachToComponent(Caster->GetMesh(), AttachmentRules, FName("SkillAttach"));
        }
    }
}

void ASkillAbility::BeginCasting_Implementation()
{
    if (PlayerRef->CheckMana(SkillDetails.ManaCost))
    {
        if (SkillDetails.CastTime > 0)
        {
            class UCastBarWidget* CastBarWidget = MainWidget->DisplayCastBar(this);
            if (CastBarWidget)
                CastBarWidget->Event_Dele_CastSuccessful.AddDynamic(this, &ASkillAbility::DisplaySkill);
        }
        else
            DisplaySkill();
    }   
}

void ASkillAbility::DisplaySkill()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("DisplaySkill"));
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    if (PlayerRef)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("PlayerRef is valid"));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Current Mana: %f"), PlayerRef->GetCurrentMP()));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Mana Cost: %f"), SkillDetails.ManaCost));
        PlayerRef->SpendMP(SkillDetails.ManaCost);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Remaining Mana: %f"), PlayerRef->GetCurrentMP()));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PlayerRef is null"));
    }
    SuccessfulCast();
}

void ASkillAbility::SuccessfulCast()
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("SuccessfulCast"));
    
}

void ASkillAbility::InterruptCasting()
{   
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    Destroy();  
}


void ASkillAbility::ActivateEffect()
{
    //DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

