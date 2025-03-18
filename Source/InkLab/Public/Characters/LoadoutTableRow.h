// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"

#include "Inventory/InventoryItemBase.h"

#include "LoadoutTableRow.generated.h"

USTRUCT(BlueprintType)
struct FLoadoutTableRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    FLoadoutTableRow() = default;
    FLoadoutTableRow(UInventoryItemBase* Item, int32 Count) : Item(Item), Count(Count) {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loadout")
    TObjectPtr<UInventoryItemBase> Item{};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loadout")
    int32 Count{};
};
