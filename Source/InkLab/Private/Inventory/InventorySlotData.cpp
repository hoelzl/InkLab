// Copyright Dr. Matthias Hölzl


#include "Inventory/InventorySlotData.h"

#include "Inventory/InventoryComponent.h"

bool IsValidSlotData(const UInventorySlotData* InventorySlotData)
{
    return InventorySlotData && InventorySlotData->InventoryComponent && InventorySlotData->Item
           && InventorySlotData->Count >= 0 && InventorySlotData->SlotIndex >= 0
           && InventorySlotData->SlotIndex < InventorySlotData->InventoryComponent->Capacity;
}
