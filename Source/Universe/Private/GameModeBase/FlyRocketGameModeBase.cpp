// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/FlyRocketGameModeBase.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"

AFlyRocketGameModeBase::AFlyRocketGameModeBase()
{
}

void AFlyRocketGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFlyRocketGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFlyRocketGameModeBase::CreateControllers()
{
	const TObjectPtr<UWorld> World =  GetWorld();

	// DefaultSpawnÇÃPlayerControllerÇéÊìæÇµÇƒï€ä«
	if (TObjectPtr<APlayerController> DefaultSpawnedPC = UGameplayStatics::GetPlayerController(World, 0))
	{
		ApplyPlayerController(0, DefaultSpawnedPC);
	}
	const int32 DefaultSpawnPlayerNum = 1;

	// PlayerControllerÇTotalPlayerNumsÇè„å¿Ç…ê∂ê¨êîÇåàíËÇ∑ÇÈ
	const int32 CreatePlayerNums = FMath::Min(TotalPlayerNums, PlayerNums);

	// PlayerControllerÇê∂ê¨Ç∑ÇÈ
	for (int32 i = 0 + DefaultSpawnPlayerNum; i < CreatePlayerNums; i++)
	{
		TObjectPtr<APlayerController> SpawnPC = UGameplayStatics::CreatePlayer(World, i);
		ApplyPlayerController(i, SpawnPC);
	}
	
	for (int32 i = CreatePlayerNums; i < TotalPlayerNums; i++)
	{
		TObjectPtr<AAIController> SpawnAIC = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass());
		ApplyAIController(i, SpawnAIC);
	}
}

void AFlyRocketGameModeBase::ApplyPlayerController(const int32 ControllerID, APlayerController* PlayerController)
{
	PCList.Add(ControllerID, PlayerController);
	ApplyController(ControllerID, PlayerController);
}

void AFlyRocketGameModeBase::ApplyAIController(const int32 ControllerID, AAIController* AIController)
{
	AICList.Add(ControllerID, AIController);
	ApplyController(ControllerID, AIController);
}

void AFlyRocketGameModeBase::ApplyController(const int32 ControllerID, AController* Controller)
{
	CList.Add(ControllerID, Controller);
}
