#include "DamageNumberWidget.h"
#include "Components/TextBlock.h"

void UDamageNumberWidget::SetDamageText(float Damage)
{
    if (DamageText)
        DamageText->SetText(FText::AsNumber(Damage));
}

void UDamageNumberWidget::PlayAnimationForward()
{
    if (AnimationForward)
        PlayAnimation(AnimationForward);
}

void UDamageNumberWidget::SetPosition(FVector2D ScreenPosition)
{
    if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
        CanvasSlot->SetPosition(ScreenPosition);
}

void UDamageNumberWidget::RemoveFromViewportWithDelay(float Delay)
{
    // Delay �Ŀ� RemoveFromViewport�� ȣ���ϴ� Ÿ�̸� ����
    GetWorld()->GetTimerManager().SetTimer(RemoveTimerHandle, this, &UDamageNumberWidget::RemoveFromViewport, Delay, false);
}

void UDamageNumberWidget::RemoveFromViewport()
{
    RemoveFromParent();
}
