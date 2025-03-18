// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "DialogueChoiceWidget.generated.h"


class UInkpotChoice;
class UTextBlock;

/**
 * Single line in a choice
 */
UCLASS()
class INKLAB_API UDialogueChoiceWidget : public UUserWidget, public IUserObjectListEntry
{
    GENERATED_BODY()

public:
    explicit UDialogueChoiceWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UTextBlock> Text;

protected:
    // -- IUserObjectListEntry
    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
    // --

    UPROPERTY(BlueprintReadOnly, Category = "DialogueWidget")
    TObjectPtr<const UInkpotChoice> Choice;
};
