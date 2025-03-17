// Copyright Dr. Matthias Hölzl

#include "UI/InkLabUserWidget.h"

void UInkLabUserWidget::Show() { SetVisibility(ESlateVisibility::Visible); }
void UInkLabUserWidget::Hide() { SetVisibility(ESlateVisibility::Hidden); }

bool UInkLabUserWidget::IsVisible() const
{
    return GetVisibility() == ESlateVisibility::Visible;
}
