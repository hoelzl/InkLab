// Copyright Dr. Matthias Hölzl

#include "UI/DialogueChoiceWidget.h"

#include "Components/TextBlock.h"
#include "Inkpot/InkpotChoice.h"

UDialogueChoiceWidget::UDialogueChoiceWidget(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
    TextBlock = CreateDefaultSubobject<UTextBlock>("Text");
}

void UDialogueChoiceWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
    if (ensure(TextBlock))
    {
        if (const UInkpotChoice* NewChoice = Cast<UInkpotChoice>(ListItemObject))
        {
            Choice = NewChoice;
            TextBlock->SetText(NewChoice->GetText());
        }
    }
}
