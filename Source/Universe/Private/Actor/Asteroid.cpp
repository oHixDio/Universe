// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Asteroid.h"

#include "Components/SphereComponent.h"

AAsteroid::AAsteroid()
{
	// Mesh èâä˙ê›íË
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Root Mesh");
	RootComponent = Mesh;

	PrimaryActorTick.bCanEverTick = true;

	if (Tags.Contains("Asteroid"))
	{
		Tags.Add("Asteroid");
	}
}

void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}