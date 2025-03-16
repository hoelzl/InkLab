// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InkLabGameMode.generated.h"

UCLASS(minimalapi)
class AInkLabGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    explicit AInkLabGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
