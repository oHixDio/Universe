// Copyright Epic Games, Inc. All Rights Reserved.

#include "UniverseGameMode.h"
#include "UniverseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUniverseGameMode::AUniverseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
