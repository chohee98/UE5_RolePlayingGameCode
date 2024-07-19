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
