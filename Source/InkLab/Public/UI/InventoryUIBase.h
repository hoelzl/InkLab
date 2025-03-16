// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryComponent.h"
#include "InventoryUIBase.generated.h"

class UInventorySlotWidgetBase;
class UWrapBox;

UCLASS()
class INKLAB_API UInventoryUIBase : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UWrapBox* InventorySlotsContainer;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    UInventoryComponent* InventoryComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    TSubclassOf<UInventorySlotWidgetBase> SlotWidgetClass;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SetInventoryComponent(UInventoryComponent* NewInventoryComponent);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RefreshInventory();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    float MaxWeightDisplay;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    float CurrentWeightDisplay;

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnInventoryUpdated();

    TArray<UInventorySlotWidgetBase*> SlotWidgets;
};
