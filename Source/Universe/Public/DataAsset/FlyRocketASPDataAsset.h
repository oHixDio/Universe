// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FlyRocketASPDataAsset.generated.h"

USTRUCT(Blueprintable)
struct FAsteroidSpawnPattern
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, EditFixedSize)
	TArray<bool> bIsSpawnList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ArraySize{ 5 };

	FAsteroidSpawnPattern()
	{
		bIsSpawnList.SetNum(ArraySize);
	}
};

/**
 * 
 */
UCLASS()
class UNIVERSE_API UFlyRocketASPDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FAsteroidSpawnPattern> AsteroidSpawnPatternList;

public:
	const FAsteroidSpawnPattern& ASPDataByIdx(int32 Idx)
	{
		return AsteroidSpawnPatternList[Idx];
	}

	const int32 ASPDataSize()
	{
		return AsteroidSpawnPatternList.Num();
	}
};
