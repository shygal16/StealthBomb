// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"

#include "Engine.h"
#include "Enemy_RealTest.h"
//#include "Runtime/Engine/Classes/GameFramework/Controller.h"



// Sets default values
AEnemy_RealTest::AEnemy_RealTest()
	: perceptionComponent(CreateDefaultSubobject< UAIPerceptionComponent >(TEXT("PerceptionComp")))
	, sightConfig(CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight")))
	, soundConfig(CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AI Hearing")))
	//, mController(CreateDefaultSubobject<AAIController>(TEXT("AI Controller")))
{
	
	perceptionComponent->ConfigureSense(*sightConfig);
	perceptionComponent->ConfigureSense(*soundConfig);
	perceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());
	
	perceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemy_RealTest::SenseStuff);
	
	sightConfig->SightRadius = 3000.0f;
	sightConfig->LoseSightRadius = 3500.f;
	sightConfig->PeripheralVisionAngleDegrees = 90.0f;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	perceptionComponent->ConfigureSense(*sightConfig);

	soundConfig->HearingRange = 400.0f;
	soundConfig->bUseLoSHearing = false;
	soundConfig->DetectionByAffiliation.bDetectEnemies = true;
	soundConfig->DetectionByAffiliation.bDetectNeutrals = true;
	soundConfig->DetectionByAffiliation.bDetectFriendlies = true;

	perceptionComponent->ConfigureSense(*soundConfig);

	PrimaryActorTick.bCanEverTick = true;
	//mController->SetPawn(this);
}

// Called when the game starts or when spawned
void AEnemy_RealTest::BeginPlay()
{
	Super::BeginPlay();
	mController = GetController();
	mTargetPos = GetActorLocation();
	// UGameplayStatics::GetPlayerCharacter(GetWorld(),0) vs Controller->GetControlledPawn()
	//UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, sightConfig->GetSenseImplementation(), this);
	//UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, sightConfig->GetSenseImplementation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, soundConfig->GetSenseImplementation(),this);

}
// Called every frame
void AEnemy_RealTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator rot = FRotator(0, 1, 0);
	SetActorRotation(GetActorRotation() + rot);
	
}

// Called to bind functionality to input
void AEnemy_RealTest::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AEnemy_RealTest::SenseStuff(TArray<AActor*> testActors)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I see you!");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "I hear you!");
//	mController->MoveToActor(testActors[0]);
	//mTargetPos = testActors[0]->GetActorLocation();
	//UNavigationSystem::SimpleMoveToActor(GetController(), testActors[0]);
	UNavigationSystem::SimpleMoveToLocation(mController,testActors[0]->GetActorLocation());
	//FVector Movement = GetActorLocation() - testActors[0]->GetActorLocation();
	//Movement /= 3;
	//SetActorLocation(GetActorLocation() - Movement);
}