// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "AIController.h"
#include "Engine.h"
#include "Enemy_RealTest.h"
//#include "Runtime/Engine/Classes/GameFramework/Controller.h"



// Sets default values
AEnemy_RealTest::AEnemy_RealTest()
	: perceptionComponent(CreateDefaultSubobject< UAIPerceptionComponent >(TEXT("PerceptionComp")))
	, sightConfig(CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight")))
	, soundConfig(CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AI Hearing")))
{
 
	
	perceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemy_RealTest::SenseStuff);

	sightConfig->SightRadius = 4000.0f;
	sightConfig->LoseSightRadius = 4020.f;
	sightConfig->PeripheralVisionAngleDegrees = 360.0f;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	soundConfig->HearingRange = 4000.0f;
	soundConfig->bUseLoSHearing = false;
	soundConfig->DetectionByAffiliation.bDetectEnemies = true;
	soundConfig->DetectionByAffiliation.bDetectNeutrals = true;
	soundConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	perceptionComponent->ConfigureSense(*sightConfig);
	perceptionComponent->ConfigureSense(*soundConfig);

	perceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());


	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy_RealTest::BeginPlay()
{
	Super::BeginPlay();
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, sightConfig->GetSenseImplementation(), Controller->GetPawn());
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, soundConfig->GetSenseImplementation(), Controller->GetPawn());
	
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

void AEnemy_RealTest::SenseStuff(TArray<AActor*> testActors)
{
	GEngine->AddOnScreenDebugMessage(-2, 15.0f, FColor::Red, "I see you!");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "I hear you!");
}