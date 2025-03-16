// Copyright Dr. Matthias Hölzl

#include "Inventory/InventoryComponent.h"

UInventoryComponent::UInventoryComponent() { PrimaryComponentTick.bCanEverTick = false; }

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initialize slots
    Slots.SetNum(Capacity);
}

bool UInventoryComponent::AddItem(UInventoryItemBase* Item, int32 Count)
{
    if (!Item || Count <= 0 || !CanAddItem(Item, Count))
    {
        return false;
    }

    int32 RemainingCount = Count;

    // First try to stack with existing items
    if (Item->CanStack())
    {
        for (int32 i = 0; i < Slots.Num() && RemainingCount > 0; i++)
        {
            if (Slots[i].Item == Item && Slots[i].Count < Item->MaxStackCount)
            {
                int32 CanAddToSlot  = FMath::Min(RemainingCount, Item->MaxStackCount - Slots[i].Count);
                Slots[i].Count     += CanAddToSlot;
                RemainingCount     -= CanAddToSlot;
            }
        }
    }

    // Add to empty slots if we still have items left
    while (RemainingCount > 0)
    {
        int32 EmptySlot = FindEmptySlot();
        if (EmptySlot == -1)
        {
            // Shouldn't happen if CanAddItem returned true
            break;
        }

        // Create a new stack
        Slots[EmptySlot].Item   = Item;
        Slots[EmptySlot].Count  = FMath::Min(RemainingCount, Item->MaxStackCount);
        RemainingCount         -= Slots[EmptySlot].Count;
    }

    OnInventoryUpdated.Broadcast();
    return true;
}

bool UInventoryComponent::RemoveItem(UInventoryItemBase* Item, int32 Count)
{
    if (!Item || Count <= 0)
    {
        return false;
    }

    int32 RemainingToRemove = Count;

    for (int32 i = Slots.Num() - 1; i >= 0 && RemainingToRemove > 0; i--)
    {
        if (Slots[i].Item == Item)
        {
            int32 AmountToRemove  = FMath::Min(RemainingToRemove, Slots[i].Count);
            Slots[i].Count       -= AmountToRemove;
            RemainingToRemove    -= AmountToRemove;

            // If we've removed all items from this slot, set it to empty
            if (Slots[i].Count <= 0)
            {
                Slots[i].Item  = nullptr;
                Slots[i].Count = 0;
            }
        }
    }

    OnInventoryUpdated.Broadcast();
    return (RemainingToRemove == 0);
}

bool UInventoryComponent::CanAddItem(UInventoryItemBase* Item, int32 Count) const
{
    if (!Item || Count <= 0)
    {
        return false;
    }

    // Check weight limit
    if (GetCurrentWeight() + (Item->Weight * Count) > MaxWeight)
    {
        return false;
    }

    int32 RemainingCount = Count;

    // First check existing stacks
    if (Item->CanStack())
    {
        for (const FInventorySlot& Slot : Slots)
        {
            if (Slot.Item == Item && Slot.Count < Item->MaxStackCount)
            {
                int32 CanAddToSlot  = FMath::Min(RemainingCount, Item->MaxStackCount - Slot.Count);
                RemainingCount     -= CanAddToSlot;

                if (RemainingCount <= 0)
                {
                    return true;
                }
            }
        }
    }

    // Count empty slots
    const int32 EmptySlotsNeeded =
        FMath::CeilToInt(static_cast<float>(RemainingCount) / static_cast<float>(Item->MaxStackCount));
    int32 EmptySlotsAvailable = 0;

    for (const FInventorySlot& Slot : Slots)
    {
        if (Slot.Item == nullptr)
        {
            EmptySlotsAvailable++;
            if (EmptySlotsAvailable >= EmptySlotsNeeded)
            {
                return true;
            }
        }
    }

    return false;
}

float UInventoryComponent::GetCurrentWeight() const
{
    float TotalWeight = 0.0f;

    for (const FInventorySlot& Slot : Slots)
    {
        if (Slot.Item)
        {
            TotalWeight += (Slot.Item->Weight * Slot.Count);
        }
    }

    return TotalWeight;
}

bool UInventoryComponent::UseItem(int32 SlotIndex)
{
    if (SlotIndex < 0 || SlotIndex >= Slots.Num() || !Slots[SlotIndex].Item || Slots[SlotIndex].Count <= 0)
    {
        return false;
    }

    UInventoryItemBase* Item = Slots[SlotIndex].Item;
    bool bWasUsed            = Item->Use(GetOwner());

    if (bWasUsed)
    {
        // Remove one item after use
        Slots[SlotIndex].Count--;

        if (Slots[SlotIndex].Count <= 0)
        {
            Slots[SlotIndex].Item  = nullptr;
            Slots[SlotIndex].Count = 0;
        }

        OnInventoryUpdated.Broadcast();
    }

    return bWasUsed;
}

bool UInventoryComponent::TransferItem(UInventoryComponent* TargetInventory, int32 SlotIndex, int32 Count)
{
    if (!TargetInventory || SlotIndex < 0 || SlotIndex >= Slots.Num() || !Slots[SlotIndex].Item
        || Slots[SlotIndex].Count <= 0 || Count <= 0)
    {
        return false;
    }

    UInventoryItemBase* Item = Slots[SlotIndex].Item;
    int32 ActualCount        = FMath::Min(Count, Slots[SlotIndex].Count);

    if (TargetInventory->CanAddItem(Item, ActualCount))
    {
        TargetInventory->AddItem(Item, ActualCount);

        // Remove the items from our inventory
        Slots[SlotIndex].Count -= ActualCount;

        if (Slots[SlotIndex].Count <= 0)
        {
            Slots[SlotIndex].Item  = nullptr;
            Slots[SlotIndex].Count = 0;
        }

        OnInventoryUpdated.Broadcast();
        return true;
    }

    return false;
}

bool UInventoryComponent::SwapSlots(int32 SourceSlotIndex, int32 TargetSlotIndex)
{
    if (SourceSlotIndex < 0 || SourceSlotIndex >= Slots.Num() || TargetSlotIndex < 0 || TargetSlotIndex >= Slots.Num()
        || SourceSlotIndex == TargetSlotIndex)
    {
        return false;
    }

    // Swap the slots
    FInventorySlot TempSlot = Slots[SourceSlotIndex];
    Slots[SourceSlotIndex]  = Slots[TargetSlotIndex];
    Slots[TargetSlotIndex]  = TempSlot;

    OnInventoryUpdated.Broadcast();
    return true;
}

bool UInventoryComponent::GetSlot(int32 SlotIndex, FInventorySlot& OutSlot) const
{
    if (SlotIndex < 0 || SlotIndex >= Slots.Num())
    {
        return false;
    }

    OutSlot = Slots[SlotIndex];
    return true;
}

int32 UInventoryComponent::FindExistingStack(const UInventoryItemBase* Item) const
{
    if (!Item || !Item->CanStack())
    {
        return -1;
    }

    for (int32 i = 0; i < Slots.Num(); i++)
    {
        if (Slots[i].Item == Item && Slots[i].Count < Item->MaxStackCount)
        {
            return i;
        }
    }

    return -1;
}

int32 UInventoryComponent::FindEmptySlot() const
{
    for (int32 i = 0; i < Slots.Num(); i++)
    {
        if (Slots[i].Item == nullptr)
        {
            return i;
        }
    }

    return -1;
}
