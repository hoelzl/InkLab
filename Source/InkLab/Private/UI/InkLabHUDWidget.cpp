// Copyright Dr. Matthias Hölzl

// GameHUD.cpp

#include "UI/InkLabHUDWidget.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "UI/DialogueWidget.h"
#include "UI/InteractionPromptWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/ReticleWidget.h"

UInkLabHUDWidget::UInkLabHUDWidget(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer} {}

void UInkLabHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Hide elements that shouldn't be visible at start
    if (InteractionPrompt)
    {
        InteractionPrompt->SetVisibility(ESlateVisibility::Hidden);
    }

    if (InventoryWidget)
    {
        InventoryWidget->Hide();
    }

    if (DialogueWidget)
    {
        DialogueWidget->Hide();
    }

    if (QuestNotificationContainer)
    {
        QuestNotificationContainer->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UInkLabHUDWidget::ShowInteractionPrompt(const FText& ActionDescription)
{
    if (ensure(InteractionPrompt))
    {
        InteractionPrompt->SetActionDescription(ActionDescription);
        InteractionPrompt->Show();
    }
}

void UInkLabHUDWidget::HideInteractionPrompt() const
{
    if (ensure(InteractionPrompt))
    {
        InteractionPrompt->Hide();
    }
}

void UInkLabHUDWidget::ShowReticle() const
{
    if (ensure(ReticleWidget))
    {
        ReticleWidget->Show();
    }
}
void UInkLabHUDWidget::HideReticle() const
{
    if (ensure(ReticleWidget))
    {
        ReticleWidget->Hide();
    }
}
void UInkLabHUDWidget::InitializeInventoryPanelData(UInventoryComponent* InventoryComponent) const
{
    if (ensure(InventoryComponent) && ensure(InventoryWidget))
    {
        InventoryWidget->InventoryComponent = InventoryComponent;
        InventoryWidget->RefreshInventory();
    }
}

void UInkLabHUDWidget::RefreshInventoryData()
{
    if (InventoryWidget)
    {
        InventoryWidget->RefreshInventory();
    }
}

bool UInkLabHUDWidget::IsInventoryPanelVisible() const
{
    if (ensure(InventoryWidget))
    {
        return InventoryWidget->IsVisible();
    }
    return false;
}

void UInkLabHUDWidget::ShowInventoryPanel()
{
    if (!ensure(InventoryWidget))
    {
        return;
    }

    InventoryWidget->SetVisibility(ESlateVisibility::Visible);
    SetInputModeForInteraction();
}

void UInkLabHUDWidget::HideInventoryPanel()
{
    if (!InventoryWidget)
    {
        return;
    }

    InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
    SetInputModeForGameplay();
}

void UInkLabHUDWidget::ToggleInventoryPanel()
{
    if (IsInventoryPanelVisible())
    {
        HideInventoryPanel();
    }
    else
    {
        ShowInventoryPanel();
    }
}

void UInkLabHUDWidget::ShowDialoguePanel()
{
    if (!ensure(DialogueWidget))
    {
        return;
    }
    DialogueWidget->Show();
    SetInputModeForInteraction();
}

void UInkLabHUDWidget::HideDialoguePanel()
{
    if (!DialogueWidget)
    {
        return;
    }
    DialogueWidget->Hide();
    SetInputModeForGameplay();
}

void UInkLabHUDWidget::ShowQuestNotification(const FText& QuestText, float Duration)
{
    if (!QuestNotificationContainer)
    {
        return;
    }

    // Create a simple text notification
    // In a real game, you'd use a dedicated quest notification widget class
    UTextBlock* QuestTextBlock = NewObject<UTextBlock>(this);
    QuestTextBlock->SetText(QuestText);
    QuestTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.8f, 0.2f, 1.0f)));

    // Remove any existing notification
    QuestNotificationContainer->ClearChildren();
    QuestNotificationContainer->AddChild(QuestTextBlock);

    // Make container visible
    QuestNotificationContainer->SetVisibility(ESlateVisibility::Visible);

    // Set timer to hide notification
    GetWorld()->GetTimerManager().ClearTimer(QuestNotificationTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(
        QuestNotificationTimerHandle, this, &UInkLabHUDWidget::HideQuestNotification, Duration, false
    );
}

void UInkLabHUDWidget::HideQuestNotification() const
{
    if (QuestNotificationContainer)
    {
        QuestNotificationContainer->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UInkLabHUDWidget::SetInputModeForInteraction() const
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetInputMode(FInputModeGameAndUI());
        PC->SetShowMouseCursor(true);
    }
}

void UInkLabHUDWidget::SetInputModeForGameplay() const
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->SetShowMouseCursor(false);
    }
}
