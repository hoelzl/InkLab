// Copyright Dr. Matthias Hölzl

#pragma once
// InventorySlotWidgetBase.h
#pragma once

#include "CoreMinimal.h"

#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryComponent.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;

UCLASS()
class INKLAB_API UInventorySlotWidget : public UUserWidget, public IUserObjectListEntry
{
    GENERATED_BODY()

public:
    explicit UInventorySlotWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    virtual void SynchronizeProperties() override;

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UTexture2D> ItemIcon;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UTextBlock> ItemCount;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UButton> SlotButton;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    TObjectPtr<const UInventorySlotData> SlotData;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UseItem();

    UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
    void OnSlotUpdated();

    // For drag and drop functionality
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool StartDrag();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool ReceiveDrop(UInventorySlotWidget* DraggedSlot);

protected:
    // -- IUserObjectListEntry
    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
    // --

    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnSlotButtonClicked();

private:
    void UpdateButtonStyle() const;
    void UpdateButtonBrush(FSlateBrush& Brush) const;
};
