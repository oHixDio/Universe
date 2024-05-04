// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FlyTrack.h"

AFlyTrack::AFlyTrack()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFlyTrack::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFlyTrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

