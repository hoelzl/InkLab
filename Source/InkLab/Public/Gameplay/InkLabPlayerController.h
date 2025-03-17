// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InkLabPlayerController.generated.h"

class AInkLabHUD;

/**
 * Player controller for the InkLab project
 */
UCLASS()
class INKLAB_API AInkLabPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
};
