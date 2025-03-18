// Copyright Dr. Matthias Hölzl


#include "UI/DialogueWidget.h"

#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Inkpot/InkpotChoice.h"

UDialogueWidget::UDialogueWidget(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
    TextBlock       = CreateDefaultSubobject<UTextBlock>("Text");
    ChoiceList = CreateDefaultSubobject<UListView>("ChoiceList");
}

void UDialogueWidget::Update(const FString& Text, TArray<UInkpotChoice*> Choices) const
{
    if (ensure(TextBlock))
    {
        TextBlock->SetText(FText::FromString(Text));
    }

    if (ensure(ChoiceList))
    {
        if (Choices.IsEmpty())
        {
            ChoiceList->ClearListItems();
        }
        else
        {
            ChoiceList->SetListItems<UInkpotChoice*>(Choices);
        }
    }
}
