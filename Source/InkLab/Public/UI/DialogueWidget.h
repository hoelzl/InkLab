// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "InkLabUserWidget.h"
#include "DialogueWidget.generated.h"


class UInkpotChoice;
class UListView;
class UTextBlock;
/**
 * Widget for displaying a Dialogue
 */
UCLASS()
class INKLAB_API UDialogueWidget : public UInkLabUserWidget
{
    GENERATED_BODY()

public:
    explicit UDialogueWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* TextBlock;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UListView* ChoiceList;

    UFUNCTION(BlueprintCallable, Category = "DialogueWidget")
    void Update(const FString& Text, TArray<UInkpotChoice*> Choices) const;
};
