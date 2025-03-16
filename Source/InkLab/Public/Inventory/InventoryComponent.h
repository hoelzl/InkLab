// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItemBase.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    UInventoryItemBase* Item = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 Count = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS(ClassGroup = (Inventory), meta = (BlueprintSpawnableComponent))
class INKLAB_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    explicit UInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnInventoryUpdated OnInventoryUpdated;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    int32 Capacity = 20;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    float MaxWeight = 100.0f;

    // Array of inventory slots
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<FInventorySlot> Slots;

    // Add an item to the inventory
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(UInventoryItemBase* Item, int32 Count = 1);

    // Remove an item from the inventory
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(UInventoryItemBase* Item, int32 Count = 1);

    // Check if we can add an item
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool CanAddItem(UInventoryItemBase* Item, int32 Count = 1) const;

    // Get the current weight of the inventory
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    float GetCurrentWeight() const;

    // Use an item from a specific slot
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool UseItem(int32 SlotIndex);

    // Transfer item to another inventory
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool TransferItem(UInventoryComponent* TargetInventory, int32 SlotIndex, int32 Count = 1);

    // Swap slots within the same inventory
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool SwapSlots(int32 SourceSlotIndex, int32 TargetSlotIndex);

    // Get a slot by index
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool GetSlot(int32 SlotIndex, FInventorySlot& OutSlot) const;

protected:
    virtual void BeginPlay() override;

    // Helper function to find existing stacks of an item
    int32 FindExistingStack(const UInventoryItemBase* Item) const;

    // Helper function to find an empty slot
    int32 FindEmptySlot() const;
};
