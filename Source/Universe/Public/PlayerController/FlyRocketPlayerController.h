// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FlyRocketPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSE_API AFlyRocketPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFlyRocketPlayerController();

	void Tick(float DeltaTime) override;
protected:
	void BeginPlay() override;

	void SetupInputComponent() override;

	void OnPossess(APawn* aPawn) override;
	
};
