// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"

#include "InkLabUserWidget.h"

#include "InteractionPromptWidget.generated.h"

class UTextBlock;
class UImage;

/**
 * Widget for displaying interaction prompts like "Press E to interact"
 */
UCLASS()
class INKLAB_API UInteractionPromptWidget : public UInkLabUserWidget
{
    GENERATED_BODY()

public:
    explicit UInteractionPromptWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void NativeConstruct() override;

    virtual void Show() override;
    virtual void Hide() override;

    // Set the prompt text
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void SetPromptFormatString(const FText& NewPromptFormatString);

    // Set the key that is shown in the prompt (e.g., "E")
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void SetInteractionKey(const FText& NewKeyText);

    // Set the action that is shown in the prompt (e.g., "interact" or "open door")
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void SetActionDescription(const FText& NewActionDescription);

protected:
    // The text block that shows the prompt
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> PromptTextBlock;

    // Default prompt format string
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FText PromptFormatString = FText::FromString("Press {key} to {action}");

    // Default interaction key text
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FText InteractionKey = FText::FromString("E");

    // Action part of the prompt
    FText ActionDescription = FText::FromString("interact");

    // Rebuild the full prompt text
    void UpdateFullPromptText() const;
};
