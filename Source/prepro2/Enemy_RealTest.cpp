// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "Enemy_RealTest.h"


// Sets default values
AEnemy_RealTest::AEnemy_RealTest()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	perceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	//sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight"));
	
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy_RealTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_RealTest::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AEnemy_RealTest::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

