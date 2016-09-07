// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "Compass.h"


// Sets default values
ACompass::ACompass()
	: mBody(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body")))
	, mRing(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring")))
	, mEmpty(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Empty")))
	, mWidgetComp(CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget")))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = mEmpty;

	mBody->AttachTo(RootComponent);
	mRing->AttachTo(RootComponent);
	mWidgetComp->AttachTo(mBody);

}

// Called when the game starts or when spawned
void ACompass::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ACompass::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

