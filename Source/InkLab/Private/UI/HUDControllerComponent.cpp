// Copyright Dr. Matthias Hölzl
// HUDControllerComponent.cpp

#include "UI/HUDControllerComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GameHUD.h"

UHUDControllerComponent::UHUDControllerComponent() { PrimaryComponentTick.bCanEverTick = false; }

void UHUDControllerComponent::BeginPlay()
{
    Super::BeginPlay();

    // Create and add the HUD to the viewport
    CreateHUD();
}

void UHUDControllerComponent::CreateHUD()
{
    if (!GameHUDClass)
    {
        return;
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC)
    {
        return;
    }

    GameHUD = CreateWidget<UGameHUD>(PC, GameHUDClass);
    if (GameHUD)
    {
        GameHUD->AddToViewport(0); // Add at lowest Z-order so it's behind everything else
    }
}

void UHUDControllerComponent::ShowInteractionPrompt(const FText& PromptText)
{
    if (GameHUD)
    {
        GameHUD->ShowInteractionPrompt(PromptText);
    }
}

void UHUDControllerComponent::HideInteractionPrompt()
{
    if (GameHUD)
    {
        GameHUD->HideInteractionPrompt();
    }
}

void UHUDControllerComponent::ToggleInventory()
{
    if (GameHUD)
    {
        GameHUD->ToggleInventoryPanel();
    }
}

void UHUDControllerComponent::SetReticleVisibility(bool bVisible)
{
    if (GameHUD)
    {
        GameHUD->SetReticleVisibility(bVisible);
    }
}

void UHUDControllerComponent::ShowDialogue(UWidget* DialogueWidget)
{
    if (GameHUD)
    {
        GameHUD->ShowDialoguePanel(DialogueWidget);
    }
}

void UHUDControllerComponent::HideDialogue()
{
    if (GameHUD)
    {
        GameHUD->HideDialoguePanel();
    }
}

void UHUDControllerComponent::ShowQuestNotification(const FText& QuestText, float Duration)
{
    if (GameHUD)
    {
        GameHUD->ShowQuestNotification(QuestText, Duration);
    }
}
