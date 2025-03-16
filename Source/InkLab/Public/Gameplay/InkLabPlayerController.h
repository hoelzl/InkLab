// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InkLabPlayerController.generated.h"

class UHUDControllerComponent;

/**
 * Player controller for the InkLab project
 */
UCLASS()
class INKLAB_API AInkLabPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    explicit AInkLabPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    UHUDControllerComponent* GetHUDController() const;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UHUDControllerComponent> HUDController;
};
