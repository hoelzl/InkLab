// Copyright Dr. Matthias Hölzl
// HUDControllerComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HUDControllerComponent.generated.h"


class UWidget;
class UGameHUD;

/**
 * Component that provides an interface for controlling the game HUD
 * Attach this to your player controller or character
 */
UCLASS(ClassGroup = (UI), meta = (BlueprintSpawnableComponent))
class INKLAB_API UHUDControllerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHUDControllerComponent();

    virtual void BeginPlay() override;

    // Reference to the created HUD widget
    UPROPERTY(BlueprintReadOnly, Category = "HUD")
    TObjectPtr<UGameHUD> GameHUD;

    // Class to use for the HUD widget
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
    TSubclassOf<UGameHUD> GameHUDClass;

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
};
