// Copyright Dr. Matthias Hölzl
// HUDControllerComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/HUD.h"

#include "InkLabHUD.generated.h"


class UInteractionTargetComponent;
class UInventoryComponent;
class UInteractionSourceComponent;
class UWidget;
class UInkLabHUDWidget;

/**
 * HUD class that manages interaction between HUD and game subsystems
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (UI), meta = (BlueprintSpawnableComponent))
class INKLAB_API AInkLabHUD : public AHUD
{
    GENERATED_BODY()

public:
    explicit AInkLabHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void BeginPlay() override;

    // Initialize connections to interaction system
    UFUNCTION(BlueprintCallable, Category = "HUD|Initialization")
    void SetupInteractionListeners(UInteractionSourceComponent* InteractionSource);

    // Initialize connections to inventory system
    UFUNCTION(BlueprintCallable, Category = "HUD|Initialization")
    void SetupInventoryListeners(UInventoryComponent* InventoryComponent);

    // Reference to the created HUD widget
    UPROPERTY(BlueprintReadOnly, Category = "HUD")
    TObjectPtr<UInkLabHUDWidget> HUDWidget;

    // Class to use for the HUD widget
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
    TSubclassOf<UInkLabHUDWidget> HUDWidgetClass;

    // Shows the interaction prompt with specified text
    UFUNCTION(BlueprintCallable, Category = "HUD|Interaction")
    void ShowInteractionPrompt(const FText& PromptText);

    // Hides the interaction prompt
    UFUNCTION(BlueprintCallable, Category = "HUD|Interaction")
    void HideInteractionPrompt();

    // Toggles the inventory UI
    UFUNCTION(BlueprintCallable, Category = "HUD|Inventory")
    void ToggleInventory();

    // Shows or hides the reticle
    UFUNCTION(BlueprintCallable, Category = "HUD|Reticle")
    void SetReticleVisibility(bool bVisible);

    // Shows a dialogue panel
    UFUNCTION(BlueprintCallable, Category = "HUD|Dialogue")
    void ShowDialogue(UWidget* DialogueWidget);

    // Hides the dialogue panel
    UFUNCTION(BlueprintCallable, Category = "HUD|Dialogue")
    void HideDialogue();

    // Shows a quest notification
    UFUNCTION(BlueprintCallable, Category = "HUD|Quest")
    void ShowQuestNotification(const FText& QuestText, float Duration = 3.0f);

protected:
    // Creates the HUD widget and adds it to the viewport
    void CreateHUD();

    UFUNCTION()
    void OnInteractionTargetFound(UInteractionTargetComponent* Target);

    UFUNCTION()
    void OnInteractionTargetLost();

    virtual void BeginDestroy() override;

    void RemoveInteractionListeners();

    UPROPERTY()
    TObjectPtr<UInteractionSourceComponent> CurrentInteractionSource;
};
