// Copyright Dr. Matthias Hölzl

// MerchantComponent.cpp

#include "Inventory/MerchantComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UMerchantComponent::UMerchantComponent(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
    ActionDescription = NSLOCTEXT("Interaction", "MerchantPrompt", "Trade");

    // Create an inventory component for the merchant
    MerchantInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("MerchantInventory"));
}

void UMerchantComponent::BeginPlay()
{
    Super::BeginPlay();

    // Set up the merchant inventory
    if (!MerchantInventory)
    {
        MerchantInventory = NewObject<UInventoryComponent>(GetOwner());
        MerchantInventory->RegisterComponent();
    }
}

void UMerchantComponent::OnInteract(UInteractionSourceComponent* Source)
{
    Super::OnInteract(Source);

    // TODO: Maybe get PC from Source? But that makes UInteractionsSourceComponent more complex...
    // Or define a helper function that checks the type of Outer and then dispatches?
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC || !MerchantUIClass)
    {
        return;
    }

    // Create the merchant UI
    if (!CurrentMerchantUI)
    {
        CurrentMerchantUI = CreateWidget<UUserWidget>(PC, MerchantUIClass);
    }

    if (CurrentMerchantUI && !CurrentMerchantUI->IsInViewport())
    {
        CurrentMerchantUI->AddToViewport();
    }
}

ESalesResult
UMerchantComponent::BuyItemFromPlayer(UInventoryComponent* PlayerInventory, int32 PlayerSlotIndex, int32 Count)
{
    if (!PlayerInventory || !MerchantInventory || PlayerSlotIndex < 0
        || PlayerSlotIndex >= PlayerInventory->Slots.Num())
    {
        return ESalesResult::BadSalesAttempt;
    }

    FInventorySlot PlayerSlot = PlayerInventory->GetSlot(PlayerSlotIndex);
    if (!PlayerSlot.IsValid() || !PlayerSlot.Item || PlayerSlot.Count < Count)
    {
        return ESalesResult::NotAvailable;
    }

    UInventoryItemBase* ItemToBuy = PlayerSlot.Item;
    int32 BuyPrice                = GetBuyPriceForItem(ItemToBuy, Count);

    if (MerchantInventory->GetMoney() < BuyPrice)
    {
        BuyPrice = MerchantInventory->GetMoney();
    }

    if (MerchantInventory->CanAddItem(ItemToBuy, Count))
    {
        if (PlayerInventory->RemoveItem(ItemToBuy, Count))
        {
            MerchantInventory->AddItem(ItemToBuy, Count);
            PlayerInventory->RemoveMoney(BuyPrice);
            MerchantInventory->AddMoney(BuyPrice);
            return ESalesResult::Success;
        }
        else
        {
            return ESalesResult::NotAvailable;
        }
    }
    else
    {
        return ESalesResult::MerchantHasNoSpace;
    }
}

ESalesResult
UMerchantComponent::SellItemToPlayer(UInventoryComponent* PlayerInventory, int32 MerchantSlotIndex, int32 Count)
{
    if (!PlayerInventory || !MerchantInventory || MerchantSlotIndex < 0
        || MerchantSlotIndex >= MerchantInventory->Slots.Num())
    {
        return ESalesResult::BadSalesAttempt;
    }

    FInventorySlot MerchantSlot = MerchantInventory->GetSlot(MerchantSlotIndex);
    if (!MerchantSlot.IsValid() || !MerchantSlot.Item || MerchantSlot.Count < Count)
    {
        return ESalesResult::NotAvailable;
    }

    UInventoryItemBase* ItemToSell = MerchantSlot.Item;
    int32 SellPrice                = GetSellPriceForItem(ItemToSell, Count);

    if (SellPrice > PlayerInventory->GetMoney())
    {
        return ESalesResult::NotEnoughMoney;
    }

    if (PlayerInventory->CanAddItem(ItemToSell, Count))
    {
        if (MerchantInventory->RemoveItem(ItemToSell, Count))
        {
            PlayerInventory->AddItem(ItemToSell, Count);
            PlayerInventory->RemoveMoney(SellPrice);
            MerchantInventory->AddMoney(SellPrice);
            return ESalesResult::Success;
        }
        else
        {
            return ESalesResult::NotAvailable;
        }
    }
    else
    {
        return ESalesResult::PlayerHasNoSpace;
    }
}

int32 UMerchantComponent::GetBuyPriceForItem(UInventoryItemBase* Item, int32 Count) const
{
    if (!Item)
    {
        return 0;
    }

    return FMath::FloorToInt(Item->Value * BuyValueMultiplier * Count);
}

int32 UMerchantComponent::GetSellPriceForItem(UInventoryItemBase* Item, int32 Count)
{
    if (!Item)
    {
        return 0;
    }

    return FMath::CeilToInt(Item->Value * SellValueMultiplier * Count);
}
