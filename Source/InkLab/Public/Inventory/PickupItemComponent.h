// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interaction/InteractionTargetComponent.h"
#include "InventoryItemBase.h"
#include "PickupItemComponent.generated.h"

UCLASS(ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class INKLAB_API UPickupItemComponent : public UInteractionTargetComponent
{
    GENERATED_BODY()

public:
    explicit UPickupItemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void OnInteract(UInteractionSourceComponent* Source) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    TObjectPtr<UInventoryItemBase> ItemToPickup = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    int32 ItemCount = 1;

protected:
    virtual void BeginPlay() override;
};
