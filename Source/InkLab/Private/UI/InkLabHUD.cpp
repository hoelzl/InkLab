// Copyright Dr. Matthias Hölzl
// HUDControllerComponent.cpp

#include "UI/InkLabHUD.h"

#include "Blueprint/UserWidget.h"
#include "Interaction/InteractionSourceComponent.h"
#include "Interaction/InteractionTargetComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InkLabHUDWidget.h"

AInkLabHUD::AInkLabHUD(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
    static ConstructorHelpers::FClassFinder<UInkLabHUDWidget> HUDClassFinder{TEXT("/Game/UI/Widgets/HUD/WBP_InkLabHUDWidget")};
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

void AInkLabHUD::SetupInteractionListeners(UInteractionSourceComponent* NewInteractionSource)
{
    RemoveInteractionListeners();

    InteractionSourceComponent = NewInteractionSource;

    if (InteractionSourceComponent)
    {
        InteractionSourceComponent->OnInteractionTargetFound.AddDynamic(this, &AInkLabHUD::OnInteractionTargetFound);
        InteractionSourceComponent->OnInteractionTargetLost.AddDynamic(this, &AInkLabHUD::OnInteractionTargetLost);
    }
}

void AInkLabHUD::SetupInventoryListeners(UInventoryComponent* NewInventoryComponent)
{
    RemoveInventoryListeners();

    InventoryComponent = NewInventoryComponent;

    if (InventoryComponent)
    {
        if (ensure(HUDWidget))
        {
            HUDWidget->InitializeInventoryPanelData(InventoryComponent);
        }
        InventoryComponent->OnInventoryUpdated.AddDynamic(this, &AInkLabHUD::OnInventoryUpdated);
    }
}

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
        InitializeWidgetInventoryData(PC);
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
        HUDWidget->ToggleInventoryPanel();
    }
}

void AInkLabHUD::SetReticleVisibility(bool bVisible)
{
    if (HUDWidget)
    {
        HUDWidget->ShowReticle();
    }
}

void AInkLabHUD::ShowDialogue()
{
    if (HUDWidget)
    {
        HUDWidget->ShowDialoguePanel();
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
void AInkLabHUD::OnInventoryUpdated()
{
    if (HUDWidget)
    {
        HUDWidget->RefreshInventoryData();
    }
}

void AInkLabHUD::BeginDestroy()
{
    RemoveInteractionListeners();
    InteractionSourceComponent = nullptr;

    Super::BeginDestroy();
}

void AInkLabHUD::RemoveInteractionListeners()
{
    if (InteractionSourceComponent)
    {
        InteractionSourceComponent->OnInteractionTargetFound.RemoveDynamic(this, &AInkLabHUD::OnInteractionTargetFound);
        InteractionSourceComponent->OnInteractionTargetLost.RemoveDynamic(this, &AInkLabHUD::OnInteractionTargetLost);
    }
}

void AInkLabHUD::InitializeWidgetInventoryData(const APlayerController* PC) const
{
    // if (const AInkLabCharacter* Character = PC->GetPawn<AInkLabCharacter>(); ensure(Character))
    // {
    //     if (UInventoryComponent* InventoryComponent = Character->GetInventoryComponent(); ensure(InventoryComponent))
    //     {
    //         HUDWidget->InitializeInventoryPanelData(InventoryComponent);
    //     }
    // }
}

void AInkLabHUD::RemoveInventoryListeners()
{
    if (InventoryComponent)
    {
        InventoryComponent->OnInventoryUpdated.RemoveDynamic(this, &AInkLabHUD::OnInventoryUpdated);
    }
}
