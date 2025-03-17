// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InkLabHUDWidget.generated.h"

class UReticleWidget;
class UInteractionPromptWidget;
class UWidget;
class UCanvasPanel;
class UOverlay;

/**
 * Main HUD widget that controls all UI elements for the game
 */
UCLASS()
class INKLAB_API UInkLabHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    explicit UInkLabHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // Sets the interaction prompt text and makes it visible
    UFUNCTION(BlueprintCallable, Category = "HUD|Interaction")
    void ShowInteractionPrompt(const FText& ActionDescription);

    // Hides the interaction prompt
    UFUNCTION(BlueprintCallable, Category = "HUD|Interaction")
    void HideInteractionPrompt() const;

    // Shows the reticle
    UFUNCTION(BlueprintCallable, Category = "HUD|Reticle")
    void ShowReticle() const;

    UFUNCTION(BlueprintCallable, Category = "HUD|Reticle")
    void HideReticle() const;

    // Checks whether the inventory UI is visible
    UFUNCTION(BlueprintCallable, Category = "HUD|Inventory")
    bool IsInventoryPanelVisible() const;

    // Shows the inventory UI
    UFUNCTION(BlueprintCallable, Category = "HUD|Inventory")
    void ShowInventoryPanel();

    // Hides the inventory UI
    UFUNCTION(BlueprintCallable, Category = "HUD|Inventory")
    void HideInventoryPanel();

    UFUNCTION(BlueprintCallable, Category = "HUD|Inventory")
    void ToggleInventoryPanel();

    // Shows a dialogue panel
    UFUNCTION(BlueprintCallable, Category = "HUD|Dialogue")
    void ShowDialoguePanel(UWidget* DialogueWidget);

    // Hides the dialogue panel
    UFUNCTION(BlueprintCallable, Category = "HUD|Dialogue")
    void HideDialoguePanel();

    // Shows a quest notification
    UFUNCTION(BlueprintCallable, Category = "HUD|Quest")
    void ShowQuestNotification(const FText& QuestText, float Duration = 3.0f);

protected:
    // Main container for all HUD elements
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UCanvasPanel> RootCanvas;

    // Container for inventory UI
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UOverlay> InventoryContainer;

    // Container for dialogue UI
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UOverlay> DialogueContainer;

    // Container for quest notifications
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UOverlay> QuestNotificationContainer;

    // Reference to the reticle widget
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UReticleWidget> ReticleWidget;

    // Reference to the interaction prompt
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UInteractionPromptWidget> InteractionPrompt;

    // Class to use for inventory widget
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Configuration")
    TSubclassOf<UUserWidget> InventoryWidgetClass;

    // References to dynamically created widgets
    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentInventoryWidget;

    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentDialogueWidget;

    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentQuestNotificationWidget;

    // Timer handle for auto-hiding quest notifications
    FTimerHandle QuestNotificationTimerHandle;

    // Hide quest notification after timer expires
    UFUNCTION()
    void HideQuestNotification() const;
};
