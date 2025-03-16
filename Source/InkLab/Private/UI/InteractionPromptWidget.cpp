// Copyright Dr. Matthias Hölzl
// InteractionPromptWidget.cpp

#include "UI/InteractionPromptWidget.h"
#include "Components/TextBlock.h"

UInteractionPromptWidget::UInteractionPromptWidget(const FObjectInitializer& ObjectInitializer)
    : Super{ObjectInitializer}
{
    CurrentActionText = FText::FromString("interact");
}

void UInteractionPromptWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (KeyTextBlock)
    {
        KeyTextBlock->SetText(DefaultKeyText);
    }

    UpdateFullPromptText();
}

void UInteractionPromptWidget::SetPromptText(const FText& NewPromptText)
{
    CurrentActionText = NewPromptText;
    UpdateFullPromptText();
}

void UInteractionPromptWidget::SetInteractionKey(const FText& NewKeyText)
{
    if (KeyTextBlock)
    {
        KeyTextBlock->SetText(NewKeyText);
    }

    UpdateFullPromptText();
}

void UInteractionPromptWidget::UpdateFullPromptText()
{
    if (!PromptTextBlock)
    {
        return;
    }

    FFormatNamedArguments Args;
    Args.Add("key", KeyTextBlock ? KeyTextBlock->GetText() : DefaultKeyText);
    Args.Add("action", CurrentActionText);

    FText FormattedText = FText::Format(PromptFormatText, Args);
    PromptTextBlock->SetText(FormattedText);
}
