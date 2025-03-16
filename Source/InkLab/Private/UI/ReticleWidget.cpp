// Copyright Dr. Matthias Hölzl
// ReticleWidget.cpp

#include "UI/ReticleWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "InkLab/InkLab.h" // For reticle position constants

UReticleWidget::UReticleWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UReticleWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ReticleImage)
    {
        ReticleImage->SetColorAndOpacity(DefaultColor);
    }

    PositionReticle();
}

void UReticleWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (ReticleImage)
    {
        ReticleImage->SetColorAndOpacity(DefaultColor);
    }
}

void UReticleWidget::SetReticleColor(const FLinearColor& NewColor) const
{
    if (ReticleImage)
    {
        ReticleImage->SetColorAndOpacity(NewColor);
    }
}

void UReticleWidget::SetReticleSize(float NewSize)
{
    Size = NewSize;

    if (ReticleImage)
    {
        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ReticleImage->Slot))
        {
            const FVector2D CurrentSize = CanvasSlot->GetSize();
            CanvasSlot->SetSize(CurrentSize * Size);
        }
    }
}

void UReticleWidget::PositionReticle()
{
    if (!ReticleImage)
    {
        return;
    }

    UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ReticleImage->Slot);
    if (!CanvasSlot)
    {
        return;
    }

    // Get viewport size
    const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);

    // Calculate position based on the global constants
    const float XPos = ViewportSize.X * AimReticleXPositionMultiplier;
    const float YPos = ViewportSize.Y * AimReticleYPositionMultiplier;

    // Set the position (centered on the calculated point)
    const FVector2D ReticleSize = CanvasSlot->GetSize();
    CanvasSlot->SetPosition(FVector2D(XPos - ReticleSize.X * 0.5f, YPos - ReticleSize.Y * 0.5f));
}
