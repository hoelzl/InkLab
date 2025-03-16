// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionPromptWidget.generated.h"

class UTextBlock;
class UImage;

/**
 * Widget for displaying interaction prompts like "Press E to interact"
 */
UCLASS()
class INKLAB_API UInteractionPromptWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    explicit UInteractionPromptWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;

    // Set the prompt text
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void SetPromptText(const FText& NewPromptText);

    // Set the key that is shown in the prompt (e.g., "E")
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void SetInteractionKey(const FText& NewKeyText);

protected:
    // The text block that shows the prompt
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> PromptTextBlock;

    // The text block that shows the key to press
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> KeyTextBlock;

    // Optional key icon image
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UImage> KeyIcon;

    // Default interaction key text
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FText DefaultKeyText = FText::FromString("E");

    // Default prompt format string
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FText PromptFormatText = FText::FromString("Press {0} to {1}");

    // Current action part of the prompt
    FText CurrentActionText;

    // Rebuild the full prompt text
    void UpdateFullPromptText();
};
