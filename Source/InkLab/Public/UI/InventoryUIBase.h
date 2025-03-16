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
    explicit UInventoryUIBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UWrapBox> InventorySlotsContainer;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    TObjectPtr<UInventoryComponent> InventoryComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    TSubclassOf<UInventorySlotWidgetBase> SlotWidgetClass;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SetInventoryComponent(UInventoryComponent* NewInventoryComponent);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RefreshInventory();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    float MaxWeightDisplay{0.f};

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    float CurrentWeightDisplay{0.f};

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnInventoryUpdated();

    TArray<UInventorySlotWidgetBase*> SlotWidgets;
};
