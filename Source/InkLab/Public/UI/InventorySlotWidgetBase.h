// Copyright Dr. Matthias Hölzl

#pragma once
// InventorySlotWidgetBase.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryComponent.h"
#include "InventorySlotWidgetBase.generated.h"

class UImage;
class UTextBlock;
class UButton;

UCLASS()
class INKLAB_API UInventorySlotWidgetBase : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UImage> ItemIcon;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UTextBlock> ItemCount;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UButton> SlotButton;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    UInventoryComponent* InventoryRef;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    int32 SlotIndex;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    FInventorySlot CurrentSlot;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UpdateSlot(const FInventorySlot& NewSlot);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UseItem();

    UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
    void OnSlotUpdated();

    // For drag and drop functionality
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool StartDrag();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool ReceiveDrop(UInventorySlotWidgetBase* DraggedSlot);

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnSlotButtonClicked();
};
