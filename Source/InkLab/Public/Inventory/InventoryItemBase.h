// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryItemBase.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Weapon,
    Armor,
    Consumable,
    Quest,
    Material,
    Miscellaneous
};

UCLASS(BlueprintType, Blueprintable)
class INKLAB_API UInventoryItemBase : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    explicit UInventoryItemBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    TObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    int32 MaxStackCount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    int32 Value;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    float Weight;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    TSubclassOf<AActor> WorldActorClass;

    UFUNCTION(BlueprintCallable, Category = "Item")
    bool CanStack() const { return MaxStackCount > 1; }

    // Override this to provide custom functionality when item is used
    UFUNCTION(BlueprintNativeEvent, Category = "Item")
    bool Use(AActor* User);
    virtual bool Use_Implementation(AActor* User);

    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
