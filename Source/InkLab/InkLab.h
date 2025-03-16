// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// The position of the aiming reticle as multiplier of the screen size.
// For third-person games, offsetting y slightly seems to result in better results,
// but it has to be done consistently in the UI and, e.g. in the interaction and
// combat systems to avoid problems.
constexpr float AimReticleXPositionMultiplier = 0.5f;
constexpr float AimReticleYPositionMultiplier = 0.48f;
