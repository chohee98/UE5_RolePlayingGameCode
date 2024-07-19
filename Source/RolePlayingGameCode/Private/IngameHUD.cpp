#include "IngameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AIngameHUD::AIngameHUD()
{
	static ConstructorHelpers::FClassFinder<UUIMainWidget> MainHUDWidgetAsset(TEXT("WidgetBlueprint'/Game/RPG/Widget/InGame/MainCharacterWidget.MainCharacterWidget_C'"));
	static ConstructorHelpers::FClassFinder<UDamageNumberWidget> DamageNumberWidgetBPClass(TEXT("WidgetBlueprint'/Game/RPG/Widget/InGame/Skill/DamageNumber.DamageNumber_C'"));

	if (MainHUDWidgetAsset.Succeeded())
		MainHUDWidgetClass = MainHUDWidgetAsset.Class;
	if (DamageNumberWidgetBPClass.Succeeded())
		DamageNumberWidgetClass = DamageNumberWidgetBPClass.Class;
}

void AIngameHUD::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* pPC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	pPC->SetInputMode(FInputModeGameAndUI());
	pPC->SetShowMouseCursor(true);

	if (IsValid(MainHUDWidgetClass))
	{
		MainHUDWidget = Cast<UUIMainWidget>(CreateWidget(GetWorld(), MainHUDWidgetClass));

		if (IsValid(MainHUDWidget))
			MainHUDWidget->AddToViewport();
	}

}

void AIngameHUD::ShowDamageNumber(float Damage, FVector Location)
{
	// 기존의 사용되지 않는 위젯을 정리
	CleanupDamageWidgets();

	if (DamageNumberWidgetClass)
	{
		UDamageNumberWidget* DamageWidget = nullptr;

		// 기존에 사용 가능한 위젯이 있는지 확인
		for (UDamageNumberWidget* Widget : ActiveDamageWidgets)
		{
			if (!Widget->IsInViewport())
			{
				DamageWidget = Widget;
				break;
			}
		}

		// 사용 가능한 위젯이 없으면 새로 생성
		if (!DamageWidget)
		{
			DamageWidget = CreateWidget<UDamageNumberWidget>(GetWorld(), DamageNumberWidgetClass);
			ActiveDamageWidgets.Add(DamageWidget);
		}

		if (DamageWidget)
		{
			DamageWidget->SetDamageText(Damage);
			DamageWidget->AddToViewport();

			// 위치를 스크린 좌표로 변환
			FVector2D ScreenPosition;
			UGameplayStatics::ProjectWorldToScreen(GetOwningPlayerController(), Location, ScreenPosition);

			// 위젯의 위치 설정 (임시적으로 설정하는 방식)
			DamageWidget->SetPositionInViewport(ScreenPosition);

			// 애니메이션 재생
			DamageWidget->PlayAnimationForward();
		}
	}
}

void AIngameHUD::CleanupDamageWidgets()
{
	for (int32 i = ActiveDamageWidgets.Num() - 1; i >= 0; i--)
	{
		if (ActiveDamageWidgets[i] && !ActiveDamageWidgets[i]->IsInViewport())
			ActiveDamageWidgets.RemoveAt(i);
	}
}
