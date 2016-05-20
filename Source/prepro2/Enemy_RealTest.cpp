// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"

#include "Engine.h"
#include "Enemy_RealTest.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
//#include "Runtime/Engine/Classes/GameFramework/Controller.h"



// Sets default values
AEnemy_RealTest::AEnemy_RealTest()
	//, mController(CreateDefaultSubobject<AAIController>(TEXT("AI Controller")))
{

	

	PrimaryActorTick.bCanEverTick = true;
	//mController->SetPawn(this);
}

// Called when the game starts or when spawned
void AEnemy_RealTest::BeginPlay()
{
	Super::BeginPlay();
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