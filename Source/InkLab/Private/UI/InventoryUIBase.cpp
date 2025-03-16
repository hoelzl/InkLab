// Copyright Dr. Matthias Hölzl


#include "UI/InventoryUIBase.h"
#include "Components/WrapBox.h"
#include "UI/InventorySlotWidgetBase.h"

void UInventoryUIBase::NativeConstruct()
{
    Super::NativeConstruct();

    if (InventoryComponent)
    {
        InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryUIBase::OnInventoryUpdated);
        RefreshInventory();
    }
}

void UInventoryUIBase::SetInventoryComponent(UInventoryComponent* NewInventoryComponent)
{
    if (InventoryComponent)
    {
        InventoryComponent->OnInventoryUpdated.RemoveDynamic(this, &UInventoryUIBase::OnInventoryUpdated);
    }

    InventoryComponent = NewInventoryComponent;

    if (InventoryComponent)
    {
        MaxWeightDisplay = InventoryComponent->MaxWeight;
        InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryUIBase::OnInventoryUpdated);
        RefreshInventory();
    }
}

void UInventoryUIBase::RefreshInventory()
{
    if (!InventoryComponent || !InventorySlotsContainer || !SlotWidgetClass)
    {
        return;
    }

    // Clear existing slots
    InventorySlotsContainer->ClearChildren();
    SlotWidgets.Empty();

    // Create new slots
    for (int32 i = 0; i < InventoryComponent->Slots.Num(); i++)
    {
        if (UInventorySlotWidgetBase* SlotWidget = CreateWidget<UInventorySlotWidgetBase>(this, SlotWidgetClass))
        {
            SlotWidget->InventoryRef = InventoryComponent;
            SlotWidget->SlotIndex    = i;
            SlotWidget->UpdateSlot(InventoryComponent->Slots[i]);

            InventorySlotsContainer->AddChild(SlotWidget);
            SlotWidgets.Add(SlotWidget);
        }
    }

    CurrentWeightDisplay = InventoryComponent->GetCurrentWeight();
}

void UInventoryUIBase::OnInventoryUpdated()
{
    // Update existing slot widgets without recreating them
    if (InventoryComponent)
    {
        for (int32 i = 0; i < SlotWidgets.Num() && i < InventoryComponent->Slots.Num(); i++)
        {
            if (SlotWidgets[i])
            {
                SlotWidgets[i]->UpdateSlot(InventoryComponent->Slots[i]);
            }
        }

        CurrentWeightDisplay = InventoryComponent->GetCurrentWeight();
    }
}
