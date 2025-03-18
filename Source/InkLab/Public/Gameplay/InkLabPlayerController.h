// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InkLabPlayerController.generated.h"


class UDialogueComponent;
class AInkLabHUD;

/**
 * Player controller for the InkLab project
 */
UCLASS()
class INKLAB_API AInkLabPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    explicit AInkLabPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
    TObjectPtr<UDialogueComponent> DialogueComponent;
};
