// Copyright Dr. Matthias Hölzl
// InteractionPromptWidget.cpp

#include "UI/InteractionPromptWidget.h"
#include "Components/TextBlock.h"

UInteractionPromptWidget::UInteractionPromptWidget(const FObjectInitializer& ObjectInitializer)
    : Super{ObjectInitializer}
{
    ActionDescription = FText::FromString("interact");
}

void UInteractionPromptWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UpdateFullPromptText();
}
void UInteractionPromptWidget::Show()
{
    UpdateFullPromptText();
    Super::Show();
}
void UInteractionPromptWidget::Hide()
{
    Super::Hide();
    // Ensure that we have a useful action description and don't keep the last target's description around
    // if somebody doesn't properly set the description.
    ActionDescription = FText::FromString("interact");
}

void UInteractionPromptWidget::SetPromptFormatString(const FText& NewPromptFormatString)
{
    ActionDescription = NewPromptFormatString;
    UpdateFullPromptText();
}

void UInteractionPromptWidget::SetInteractionKey(const FText& NewKeyText)
{
    InteractionKey = NewKeyText;
    UpdateFullPromptText();
}
void UInteractionPromptWidget::SetActionDescription(const FText& NewActionDescription)
{
    ActionDescription = NewActionDescription;
    UpdateFullPromptText();
}

void UInteractionPromptWidget::UpdateFullPromptText() const
{
    if (!PromptTextBlock)
    {
        return;
    }

    FFormatNamedArguments Args;
    Args.Add("key", InteractionKey);
    Args.Add("action", ActionDescription);

    const FText FormattedText = FText::Format(PromptFormatString, Args);
    PromptTextBlock->SetText(FormattedText);
}
