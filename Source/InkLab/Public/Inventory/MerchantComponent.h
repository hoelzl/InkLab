// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interaction/InteractionTargetComponent.h"
#include "Inventory/InventoryComponent.h"
#include "MerchantComponent.generated.h"

UCLASS(ClassGroup = (Inventory), meta = (BlueprintSpawnableComponent))
class INKLAB_API UMerchantComponent : public UInteractionTargetComponent
{
    GENERATED_BODY()

public:
    explicit UMerchantComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void OnInteract(UInteractionSourceComponent* Source) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Merchant")
    TObjectPtr<UInventoryComponent> MerchantInventory;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Merchant")
    TSubclassOf<UUserWidget> MerchantUIClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Merchant")
    float BuyValueMultiplier = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Merchant")
    float SellValueMultiplier = 1.5f;

    UFUNCTION(BlueprintCallable, Category = "Merchant")
    bool BuyItemFromPlayer(UInventoryComponent* PlayerInventory, int32 PlayerSlotIndex, int32 Count = 1);

    UFUNCTION(BlueprintCallable, Category = "Merchant")
    bool SellItemToPlayer(UInventoryComponent* PlayerInventory, int32 MerchantSlotIndex, int32 Count = 1);

    UFUNCTION(BlueprintCallable, Category = "Merchant")
    int32 GetBuyPriceForItem(UInventoryItemBase* Item, int32 Count = 1) const;

    UFUNCTION(BlueprintCallable, Category = "Merchant")
    int32 GetSellPriceForItem(UInventoryItemBase* Item, int32 Count = 1);

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentMerchantUI;
};
