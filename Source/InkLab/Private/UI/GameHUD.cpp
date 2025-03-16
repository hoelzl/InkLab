// Copyright Dr. Matthias Hölzl

// GameHUD.cpp

#include "UI/GameHUD.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"
#include "UI/InteractionPromptWidget.h"
#include "UI/ReticleWidget.h"

UGameHUD::UGameHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGameHUD::NativeConstruct()
{
    Super::NativeConstruct();

    // Hide elements that shouldn't be visible at start
    if (InteractionPrompt)
    {
        InteractionPrompt->SetVisibility(ESlateVisibility::Hidden);
    }

    if (InventoryContainer)
    {
        InventoryContainer->SetVisibility(ESlateVisibility::Hidden);
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

void UGameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Any per-frame HUD updates can go here
}

void UGameHUD::ShowInteractionPrompt(const FText& PromptText)
{
    if (InteractionPrompt)
    {
        InteractionPrompt->SetPromptText(PromptText);
        InteractionPrompt->SetVisibility(ESlateVisibility::Visible);
    }
}

void UGameHUD::HideInteractionPrompt() const
{
    if (InteractionPrompt)
    {
        InteractionPrompt->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UGameHUD::SetReticleVisibility(bool bVisible) const
{
    if (ReticleWidget)
    {
        ReticleWidget->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void UGameHUD::ToggleInventoryPanel()
{
    if (!InventoryContainer)
    {
        return;
    }

    if (InventoryContainer->GetVisibility() == ESlateVisibility::Visible)
    {
        InventoryContainer->SetVisibility(ESlateVisibility::Hidden);

        // Enable gameplay input when inventory is closed
        if (APlayerController* PC = GetOwningPlayer())
        {
            PC->SetInputMode(FInputModeGameOnly());
            PC->SetShowMouseCursor(false);
        }
    }
    else
    {
        // Create inventory widget if it doesn't exist
        if (!CurrentInventoryWidget && InventoryWidgetClass)
        {
            CurrentInventoryWidget = CreateWidget<UUserWidget>(this, InventoryWidgetClass);
            if (CurrentInventoryWidget)
            {
                InventoryContainer->AddChild(CurrentInventoryWidget);

                // Configure overlay slot to fill parent
                if (UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(CurrentInventoryWidget->Slot))
                {
                    OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
                    OverlaySlot->SetVerticalAlignment(VAlign_Fill);
                }
            }
        }

        InventoryContainer->SetVisibility(ESlateVisibility::Visible);

        // Enable UI input when inventory is open
        if (APlayerController* PC = GetOwningPlayer())
        {
            PC->SetInputMode(FInputModeGameAndUI());
            PC->SetShowMouseCursor(true);
        }
    }
}

void UGameHUD::ShowDialoguePanel(UWidget* DialogueWidget)
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

void UGameHUD::HideDialoguePanel()
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

void UGameHUD::ShowQuestNotification(const FText& QuestText, float Duration)
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
        QuestNotificationTimerHandle, this, &UGameHUD::HideQuestNotification, Duration, false
    );
}

void UGameHUD::HideQuestNotification() const
{
    if (QuestNotificationContainer)
    {
        QuestNotificationContainer->SetVisibility(ESlateVisibility::Hidden);
    }
}
