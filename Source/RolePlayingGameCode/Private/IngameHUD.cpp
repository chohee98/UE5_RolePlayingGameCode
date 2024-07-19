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
	// ������ ������ �ʴ� ������ ����
	CleanupDamageWidgets();

	if (DamageNumberWidgetClass)
	{
		UDamageNumberWidget* DamageWidget = nullptr;

		// ������ ��� ������ ������ �ִ��� Ȯ��
		for (UDamageNumberWidget* Widget : ActiveDamageWidgets)
		{
			if (!Widget->IsInViewport())
			{
				DamageWidget = Widget;
				break;
			}
		}

		// ��� ������ ������ ������ ���� ����
		if (!DamageWidget)
		{
			DamageWidget = CreateWidget<UDamageNumberWidget>(GetWorld(), DamageNumberWidgetClass);
			ActiveDamageWidgets.Add(DamageWidget);
		}

		if (DamageWidget)
		{
			DamageWidget->SetDamageText(Damage);
			DamageWidget->AddToViewport();

			// ��ġ�� ��ũ�� ��ǥ�� ��ȯ
			FVector2D ScreenPosition;
			UGameplayStatics::ProjectWorldToScreen(GetOwningPlayerController(), Location, ScreenPosition);

			// ������ ��ġ ���� (�ӽ������� �����ϴ� ���)
			DamageWidget->SetPositionInViewport(ScreenPosition);

			// �ִϸ��̼� ���
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
