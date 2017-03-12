// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "LightDetector.h"
#include "Perception/AISense_Sight.h"

// Sets default values
ALightDetector::ALightDetector()
	: mDummy(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model")))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = mDummy;

}

// Called when the game starts or when spawned
void ALightDetector::BeginPlay()
{
	Super::BeginPlay();
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
}

// Called every frame
void ALightDetector::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	//if(mActive)
	//DrawDebugSphere(GetWorld(), GetActorLocation(), 100, 20, FColor::Red);	
}

