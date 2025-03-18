// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"

#include "Components/TextBlock.h"
#include "InkLabUserWidget.h"
#include "Inventory/InventoryComponent.h"

#include "InventoryUIBase.generated.h"


class UTileView;
class UInventorySlotWidget;
class UWrapBox;

UCLASS()
class INKLAB_API UInventoryUIBase : public UInkLabUserWidget
{
    GENERATED_BODY()

public:
    explicit UInventoryUIBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UTileView> InventorySlotsContainer;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UTextBlock> WeightTextBlock;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UTextBlock> MoneyTextBlock;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UTextBlock> CapacityTextBlock;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    TObjectPtr<UInventoryComponent> InventoryComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    FText WeightFormatString = FText::FromString("Weight: {0}/{1}");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    FText MoneyFormatString = FText::FromString("Money: {0}");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    FText CapacityFormatString = FText::FromString("Capacity: {0}/{1}");

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SetInventoryComponent(UInventoryComponent* NewInventoryComponent);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RefreshInventory();

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnInventoryUpdated();
};
