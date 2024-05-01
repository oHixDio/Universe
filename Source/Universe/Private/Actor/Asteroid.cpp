// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Asteroid.h"

#include "Components/SphereComponent.h"

AAsteroid::AAsteroid()
{
	// Mesh èâä˙ê›íË
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Root Mesh");
	RootComponent = Mesh;

	// CollisionArea èâä˙ê›íË
	CollisionArea = CreateDefaultSubobject<USphereComponent>("Collision Component");
	CollisionArea->SetupAttachment(RootComponent);

	CollisionArea->OnComponentBeginOverlap.AddDynamic(this, &AAsteroid::OnBeginOverlap);

	PrimaryActorTick.bCanEverTick = true;

}

void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAsteroid::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APawn>(OtherActor))
	{
		Destroy();
	}
}