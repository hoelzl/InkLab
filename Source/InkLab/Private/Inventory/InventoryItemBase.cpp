// Copyright Dr. Matthias Hölzl

#include "Inventory/InventoryItemBase.h"

inline UInventoryItemBase::UInventoryItemBase(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
    Name          = FText::FromString("Item");
    Description   = FText::FromString("An item that can be stored in inventory");
    Icon          = nullptr;
    ItemType      = EItemType::Miscellaneous;
    MaxStackCount = 1;
    Value         = 0;
    Weight        = 0.0f;
}

bool UInventoryItemBase::Use_Implementation(AActor* User)
{
    // Base implementation does nothing, to be overridden by child classes
    return false;
}

FPrimaryAssetId UInventoryItemBase::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(GetClass()->GetFName(), GetFName());
}
