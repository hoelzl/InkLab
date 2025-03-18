// Copyright Dr. Matthias Hölzl

#include "UI/InventoryWidget.h"

#include "Components/TileView.h"
#include "Inventory/InventorySlotData.h"
#include "UI/InventorySlotWidget.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer} {}

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (InventoryComponent)
    {
        InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::OnInventoryUpdated);
        RefreshInventory();
    }
}

void UInventoryWidget::SetInventoryComponent(UInventoryComponent* NewInventoryComponent)
{
    if (InventoryComponent)
    {
        InventoryComponent->OnInventoryUpdated.RemoveDynamic(this, &UInventoryWidget::OnInventoryUpdated);
    }

    InventoryComponent = NewInventoryComponent;

    if (InventoryComponent)
    {
        InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::OnInventoryUpdated);
        RefreshInventory();
    }
}

void UInventoryWidget::RefreshInventory()
{
    if (!InventoryComponent || !InventoryTileView)
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
    InventoryTileView->SetListItems<UInventorySlotData*>(InventoryComponent->GetAllSlotData());
}

void UInventoryWidget::OnInventoryUpdated() { RefreshInventory(); }
