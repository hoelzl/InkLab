// Copyright Dr. Matthias Hölzl
// HUDControllerComponent.cpp

#include "UI/InkLabHUD.h"

#include "Blueprint/UserWidget.h"
#include "Interaction/InteractionSourceComponent.h"
#include "Interaction/InteractionTargetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InkLabHUDWidget.h"

AInkLabHUD::AInkLabHUD(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
    static ConstructorHelpers::FClassFinder<UInkLabHUDWidget> HUDClassFinder{TEXT("/Game/UI/WBP_InkLabHUDWidget")};
    if (ensure(HUDClassFinder.Succeeded()))
    {
        HUDWidgetClass = HUDClassFinder.Class;
    }
    else
    {
        // To prevent crashes, if possible, even if we don't get a usable HUD.
        HUDWidgetClass = UInkLabHUDWidget::StaticClass();
    }
}

void AInkLabHUD::BeginPlay()
{
    Super::BeginPlay();

    // Create and add the HUD to the viewport
    CreateHUD();
}

void AInkLabHUD::SetupInteractionListeners(UInteractionSourceComponent* InteractionSource)
{
    RemoveInteractionListeners();

    CurrentInteractionSource = InteractionSource;

    if (CurrentInteractionSource)
    {
        InteractionSource->OnInteractionTargetFound.AddDynamic(this, &AInkLabHUD::OnInteractionTargetFound);
        InteractionSource->OnInteractionTargetLost.AddDynamic(this, &AInkLabHUD::OnInteractionTargetLost);
    }
}

void AInkLabHUD::SetupInventoryListeners(UInventoryComponent* InventoryComponent) {}

void AInkLabHUD::CreateHUD()
{
    if (!HUDWidgetClass)
    {
        return;
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC)
    {
        return;
    }

    HUDWidget = CreateWidget<UInkLabHUDWidget>(PC, HUDWidgetClass);
    if (HUDWidget)
    {
        HUDWidget->AddToViewport(0); // Add at lowest Z-order so it's behind everything else
    }
}

void AInkLabHUD::ShowInteractionPrompt(const FText& PromptText)
{
    if (HUDWidget)
    {
        HUDWidget->ShowInteractionPrompt(PromptText);
    }
}

void AInkLabHUD::HideInteractionPrompt()
{
    if (HUDWidget)
    {
        HUDWidget->HideInteractionPrompt();
    }
}

void AInkLabHUD::ToggleInventory()
{
    if (HUDWidget)
    {
        HUDWidget->ShowInventoryPanel();
    }
}

void AInkLabHUD::SetReticleVisibility(bool bVisible)
{
    if (HUDWidget)
    {
        HUDWidget->ShowReticle();
    }
}

void AInkLabHUD::ShowDialogue(UWidget* DialogueWidget)
{
    if (HUDWidget)
    {
        HUDWidget->ShowDialoguePanel(DialogueWidget);
    }
}

void AInkLabHUD::HideDialogue()
{
    if (HUDWidget)
    {
        HUDWidget->HideDialoguePanel();
    }
}

void AInkLabHUD::ShowQuestNotification(const FText& QuestText, float Duration)
{
    if (HUDWidget)
    {
        HUDWidget->ShowQuestNotification(QuestText, Duration);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppParameterMayBeConstPtrOrRef
void AInkLabHUD::OnInteractionTargetFound(UInteractionTargetComponent* Target)
{
    if (Target && HUDWidget)
    {
        HUDWidget->ShowInteractionPrompt(Target->ActionDescription);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AInkLabHUD::OnInteractionTargetLost()
{
    if (HUDWidget)
    {
        HUDWidget->HideInteractionPrompt();
    }
}
void AInkLabHUD::BeginDestroy()
{
    RemoveInteractionListeners();
    CurrentInteractionSource = nullptr;

    Super::BeginDestroy();
}

void AInkLabHUD::RemoveInteractionListeners()
{
    if (CurrentInteractionSource)
    {
        CurrentInteractionSource->OnInteractionTargetFound.RemoveDynamic(this, &AInkLabHUD::OnInteractionTargetFound);
        CurrentInteractionSource->OnInteractionTargetLost.RemoveDynamic(this, &AInkLabHUD::OnInteractionTargetLost);
    }
}
