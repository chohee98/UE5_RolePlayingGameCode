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

			// ��ġ�� ��ũ�� ��ǥ�� ��ȯ
			FVector2D ScreenPosition;
			UGameplayStatics::ProjectWorldToScreen(GetOwningPlayerController(), Location, ScreenPosition);

			// ������ ��ġ ���� (�ӽ������� �����ϴ� ���)
			DamageWidget->SetPositionInViewport(ScreenPosition);

			// �ִϸ��̼� ��� �� ���� �ð��� ������ ����
			DamageWidget->PlayAnimationForward();
			DamageWidget->RemoveFromViewportWithDelay(2.0f); // 2�� �� ����
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

	// ť���� ��� ������ ������ �˻�
	UDamageNumberWidget* Widget = nullptr;
	if (WidgetQueue.Dequeue(Widget))
	{
		Widget->SetActive(true);
		return Widget;
	}

	// ��� ������ ������ ������ ���� ����
	if (DamageNumberWidgetClass)
	{
		UDamageNumberWidget* NewWidget = CreateWidget<UDamageNumberWidget>(GetWorld(), DamageNumberWidgetClass);
		NewWidget->SetActive(true);
		WidgetQueue.Enqueue(NewWidget);
		return NewWidget;
	}

	return nullptr;
}
