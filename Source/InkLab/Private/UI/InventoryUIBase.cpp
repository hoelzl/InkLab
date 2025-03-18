// Copyright Dr. Matthias Hölzl

#include "UI/InventoryUIBase.h"

#include "Components/TileView.h"
#include "Inventory/InventorySlotData.h"
#include "UI/InventorySlotWidget.h"

UInventoryUIBase::UInventoryUIBase(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer} {}

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
        InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryUIBase::OnInventoryUpdated);
        RefreshInventory();
    }
}

void UInventoryUIBase::RefreshInventory()
{
    if (!InventoryComponent || !InventorySlotsContainer)
    {
        return;
    }

    if (CapacityTextBlock)
    {
        CapacityTextBlock->SetText(
            FText::Format(CapacityFormatString, InventoryComponent->GetNumFreeSlots(), InventoryComponent->Capacity)
        );
    }
    if (MoneyTextBlock)
    {
        MoneyTextBlock->SetText(FText::Format(MoneyFormatString, InventoryComponent->GetMoney()));
    }
    if (WeightTextBlock)
    {
        WeightTextBlock->SetText(
            FText::Format(WeightFormatString, InventoryComponent->GetCurrentWeight(), InventoryComponent->MaxWeight)
        );
    }

    // Update the inventory slots
    InventorySlotsContainer->SetListItems<UInventorySlotData*>(InventoryComponent->GetAllSlotData());
}

void UInventoryUIBase::OnInventoryUpdated() { RefreshInventory(); }
