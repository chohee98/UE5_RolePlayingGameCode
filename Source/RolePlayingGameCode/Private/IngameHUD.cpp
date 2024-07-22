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
	if (pPC)
	{
		pPC->SetInputMode(FInputModeGameAndUI());
		pPC->SetShowMouseCursor(true);
	}
	
	if (IsValid(MainHUDWidgetClass))
	{
		MainHUDWidget = Cast<UUIMainWidget>(CreateWidget(GetWorld(), MainHUDWidgetClass));

		if (IsValid(MainHUDWidget))
			MainHUDWidget->AddToViewport();
			
	}
}

void AIngameHUD::ShowDamageNumber(float Damage, FVector Location)
{
	UDamageNumberWidget* DamageWidget = GetPooledDamageWidget();

	if (DamageNumberWidgetClass)
	{
		if (DamageWidget)
		{
			DamageWidget->SetDamageText(Damage);
			DamageWidget->AddToViewport();

			// 위치를 스크린 좌표로 변환
			FVector2D ScreenPosition;
			UGameplayStatics::ProjectWorldToScreen(GetOwningPlayerController(), Location, ScreenPosition);

			// 위젯의 위치 설정 (임시적으로 설정하는 방식)
			DamageWidget->SetPositionInViewport(ScreenPosition);

			// 애니메이션 재생 및 일정 시간이 지나면 제거
			DamageWidget->PlayAnimationForward();
			DamageWidget->RemoveFromViewportWithDelay(2.0f); // 2초 후 제거
		}
	}
}

void AIngameHUD::CleanupDamageWidgets()
{
	/*for (int32 i = ActiveDamageWidgets.Num() - 1; i >= 0; i--)
	{
		if (ActiveDamageWidgets[i] && !ActiveDamageWidgets[i]->IsInViewport())
			ActiveDamageWidgets.RemoveAt(i);
	}*/
	while (!WidgetQueue.IsEmpty())
	{
		UDamageNumberWidget* Widget;
		WidgetQueue.Peek(Widget);

		if (Widget && !Widget->IsInViewport())
			WidgetQueue.Pop();
		else
			break;
	}
}

UDamageNumberWidget* AIngameHUD::GetPooledDamageWidget()
{
	CleanupDamageWidgets();

	// 큐에서 사용 가능한 위젯을 검색
	UDamageNumberWidget* Widget = nullptr;
	if (WidgetQueue.Dequeue(Widget))
	{
		Widget->SetActive(true);
		return Widget;
	}

	// 사용 가능한 위젯이 없으면 새로 생성
	if (DamageNumberWidgetClass)
	{
		UDamageNumberWidget* NewWidget = CreateWidget<UDamageNumberWidget>(GetWorld(), DamageNumberWidgetClass);
		NewWidget->SetActive(true);
		WidgetQueue.Enqueue(NewWidget);
		return NewWidget;
	}

	return nullptr;
}
