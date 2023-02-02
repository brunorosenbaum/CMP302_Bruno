// Copyright Epic Games, Inc. All Rights Reserved.

#include "coursework302GameMode.h"
#include "coursework302Character.h"
#include "UObject/ConstructorHelpers.h"

Acoursework302GameMode::Acoursework302GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
