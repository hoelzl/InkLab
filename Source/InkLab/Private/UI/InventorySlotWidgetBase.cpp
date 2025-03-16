// Copyright Dr. Matthias Hölzl

// InventorySlotWidgetBase.cpp

#include "UI/InventorySlotWidgetBase.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventorySlotWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidgetBase::OnSlotButtonClicked);
    }
}

void UInventorySlotWidgetBase::UpdateSlot(const FInventorySlot& NewSlot)
{
    CurrentSlot = NewSlot;

    // Update the UI elements based on the slot data
    if (ItemIcon)
    {
        if (CurrentSlot.Item && CurrentSlot.Count > 0)
        {
            ItemIcon->SetBrushFromTexture(CurrentSlot.Item->Icon);
            ItemIcon->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            ItemIcon->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (ItemCount)
    {
        if (CurrentSlot.Item && CurrentSlot.Count > 1)
        {
            ItemCount->SetText(FText::AsNumber(CurrentSlot.Count));
            ItemCount->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            ItemCount->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    OnSlotUpdated();
}

void UInventorySlotWidgetBase::UseItem()
{
    if (InventoryRef)
    {
        InventoryRef->UseItem(SlotIndex);
    }
}

void UInventorySlotWidgetBase::OnSlotButtonClicked() { UseItem(); }

bool UInventorySlotWidgetBase::StartDrag()
{
    // Can only drag if the slot has an item
    return CurrentSlot.Item != nullptr && CurrentSlot.Count > 0;
}

bool UInventorySlotWidgetBase::ReceiveDrop(UInventorySlotWidgetBase* DraggedSlot)
{
    if (!DraggedSlot || !InventoryRef || DraggedSlot->SlotIndex == SlotIndex)
    {
        return false;
    }

    // If we're in the same inventory, simply swap slots
    if (DraggedSlot->InventoryRef == InventoryRef)
    {
        return InventoryRef->SwapSlots(DraggedSlot->SlotIndex, SlotIndex);
    }
    // Otherwise, transfer item from the source inventory to this inventory
    else if (DraggedSlot->InventoryRef)
    {
        return DraggedSlot->InventoryRef->TransferItem(
            InventoryRef, DraggedSlot->SlotIndex, DraggedSlot->CurrentSlot.Count
        );
    }

    return false;
}
