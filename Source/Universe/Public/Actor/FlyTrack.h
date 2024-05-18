// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlyTrack.generated.h"

class UFlyRocketASPDataAsset;

USTRUCT(Blueprintable)
struct FTrackUpdateInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UFlyRocketASPDataAsset> AsteroidSpawnPattenData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> SpawnAsteroidClass{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SpawnOffsetZ{ 1000.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SpawnOffsetY{ 200.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DefaultSpawnAmount{ 100 };

};

UCLASS(ABSTRACT)
class UNIVERSE_API AFlyTrack : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> StartPoint{ nullptr };

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APawn> FlyRocket{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = "true"))
	FTrackUpdateInfo TrackUpdateInfo;

public:	
	AFlyTrack();

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

private:
	/*
		FTrackUpdateInfo�̏������ɁA���f���𐶐����郁���o�֐�
		TUI��BP�Ɍ��J���Ă���AEditor��ŕҏW���邱�Ƃ��\�B
		�������鏬�f���N���X���w�肵�Ă��Ȃ��ꍇ�A���̊֐��͉������Ȃ��B
	*/
	void UpdateTrack();
};


