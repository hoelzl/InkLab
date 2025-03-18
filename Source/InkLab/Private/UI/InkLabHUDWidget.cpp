// Copyright Dr. Matthias Hölzl

// GameHUD.cpp

#include "UI/InkLabHUDWidget.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"
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

    if (DialogueContainer)
    {
        DialogueContainer->SetVisibility(ESlateVisibility::Hidden);
    }

    if (QuestNotificationContainer)
    {
        QuestNotificationContainer->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UInkLabHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Any per-frame HUD updates can go here
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
    if (ensure(InventoryWidget))
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

    // Enable UI input when inventory is open
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetInputMode(FInputModeGameAndUI());
        PC->SetShowMouseCursor(true);
    }
}

void UInkLabHUDWidget::HideInventoryPanel()
{
    if (!ensure(InventoryWidget))
    {
        return;
    }

    InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

    // Enable gameplay input when inventory is closed
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->SetShowMouseCursor(false);
    }
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
void UInkLabHUDWidget::UpdateInventoryData()
{
    if (ensure(InventoryWidget))
    {
        InventoryWidget->RefreshInventory();
    }
}

void UInkLabHUDWidget::ShowDialoguePanel(UWidget* DialogueWidget)
{
    if (!DialogueContainer || !DialogueWidget)
    {
        return;
    }

    // Remove any existing dialogue widget
    if (CurrentDialogueWidget)
    {
        CurrentDialogueWidget->RemoveFromParent();
    }

    // Set the new dialogue widget
    CurrentDialogueWidget = Cast<UUserWidget>(DialogueWidget);
    if (CurrentDialogueWidget)
    {
        DialogueContainer->AddChild(CurrentDialogueWidget);

        // Configure overlay slot to fill parent
        if (UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(CurrentDialogueWidget->Slot))
        {
            OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
            OverlaySlot->SetVerticalAlignment(VAlign_Fill);
        }

        DialogueContainer->SetVisibility(ESlateVisibility::Visible);

        // Enable UI input mode when dialogue is active
        if (APlayerController* PC = GetOwningPlayer())
        {
            PC->SetInputMode(FInputModeGameAndUI());
            PC->SetShowMouseCursor(true);
        }
    }
}

void UInkLabHUDWidget::HideDialoguePanel()
{
    if (!DialogueContainer)
    {
        return;
    }

    DialogueContainer->SetVisibility(ESlateVisibility::Hidden);

    // Reset input mode when dialogue closes
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->SetShowMouseCursor(false);
    }
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
