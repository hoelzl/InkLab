// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gameplay/InkLabGameMode.h"
#include "UObject/ConstructorHelpers.h"

AInkLabGameMode::AInkLabGameMode()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass{
        TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter")
    };
    if (PlayerPawnBPClass.Class != nullptr)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}
