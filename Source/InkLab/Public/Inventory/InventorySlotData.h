// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventorySlotData.generated.h"


class UInventoryComponent;
class UInventoryItemBase;

/**
 * Data to pass to an inventory slot
 */
UCLASS(Blueprintable, BlueprintType)
class INKLAB_API UInventorySlotData : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    TObjectPtr<UInventoryComponent> InventoryComponent;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    TObjectPtr<const UInventoryItemBase> Item;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    int32 Count{};

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    int32 SlotIndex{};
};

bool IsValidSlotData(const UInventorySlotData* InventorySlotData);
