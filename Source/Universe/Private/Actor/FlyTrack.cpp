// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FlyTrack.h"

#include "DataAsset/FlyRocketASPDataAsset.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AFlyTrack::AFlyTrack()
{
	StartPoint = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = StartPoint;

	PrimaryActorTick.bCanEverTick = true;
}

void AFlyTrack::BeginPlay()
{
	Super::BeginPlay();

	UpdateTrack();
}

void AFlyTrack::UpdateTrack()
{
	// �X�|�[���N���X���w�肳��Ă��Ȃ��ꍇ�A���������I������
	if (!IsValid(TrackUpdateInfo.SpawnAsteroidClass))
	{
		return;
	}

	// 0��ڂ͔����ɍl����B1�`DefaultSpawnAmount�񂷂�
	// �c�����̃��[�v
	for (int32 z = 1; z <= TrackUpdateInfo.DefaultSpawnAmount; z++)
	{
		// �p�^�[�����X�g����p�^�[���������_���Ɏw�肷��
		int PattenIdx = FMath::RandHelper(TrackUpdateInfo.AsteroidSpawnPattenData->ASPDataSize());
		// �w�肳�ꂽ�p�^�[����ێ�
		const FAsteroidSpawnPattern& ASP = TrackUpdateInfo.AsteroidSpawnPattenData->ASPDataByIdx(PattenIdx);
		// ���ւ̃X�|�[���񐔂�ێ�
		int32 HorizSpawnNum = ASP.bIsSpawnList.Num();
		// �������̃��[�v
		for (int32 y = 0; y < HorizSpawnNum; y++)
		{
			if (ASP.bIsSpawnList[y])
			{
				// StartPoint�𒆐S�Ƃ��邽�߁AHalf�̒l������
				int32 HorizNumber = y - HorizSpawnNum / 2;

				// StartPoint��Location��Offset����Location�����Z
				FVector SpawnLocation = FVector(0, TrackUpdateInfo.SpawnOffsetY * HorizNumber, TrackUpdateInfo.SpawnOffsetZ * z) + GetActorLocation();

				// SpawnClass�͊֐��㕔��null�`�F�b�N���Ă��邽�߁A���S�Ɏg�p�ł���
				GetWorld()->SpawnActor<AActor>(TrackUpdateInfo.SpawnAsteroidClass, SpawnLocation, GetActorRotation());
			}
		}
		
	}
	
	
}

void AFlyTrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(FlyRocket))
	{
		double Dist = FVector::Dist(GetActorLocation(), FlyRocket->GetActorLocation());
		// UKismetSystemLibrary::PrintString(this, FString::FromInt(Dist / 100), true, true, FColor::Red, 2.0f, FName("Dist"));
	}
}

