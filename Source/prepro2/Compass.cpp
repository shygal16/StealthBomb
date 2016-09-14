// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "Compass.h"
#include "prepro2Character.h"
#include "Enemy_RealTest.h"


// Sets default values
ACompass::ACompass()
	: mBody(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body")))
	, mRing(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring")))
	, mEmpty(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Empty")))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = mEmpty;
	
	mBody->AttachTo(RootComponent);
	mRing->AttachTo(mBody);

}

// Called when the game starts or when spawned
void ACompass::BeginPlay()
{
	Super::BeginPlay();	
	mBlinkTimer = mBlinkRate;
}

// Called every frame
void ACompass::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (mCompassShown)
	{
		GetQuadrant(DeltaTime);
	}
	UpdateState();
}

void ACompass::UpdateState()
{
	FOutputDeviceNull ar;
	CallFunctionByNameWithArguments(TEXT("UpdateWidget"), ar, NULL, true);
}

void ACompass::GetQuadrant(float DeltaTime)
{
	Aprepro2Character* player = Cast<Aprepro2Character>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (mEnemy)
	{

			FVector2D dir = (FVector2D)mEnemy->GetActorLocation() - (FVector2D)player->GetActorLocation();
			FVector2D forward = (FVector2D)player->GetFirstPersonCameraComponent()->GetForwardVector();
			FVector2D side = (FVector2D)player->GetFirstPersonCameraComponent()->GetRightVector();
			dir.Normalize();

			int quadrant = 0;
			FVector2D diagonal;

			quadrant += FVector2D::DotProduct(dir, side) < 0 ? 0 : 1;
			quadrant += FVector2D::DotProduct(dir, forward) > 0 ? 0 : 2;
			//quadrant += FVector2D::DotProduct(dir, diagonal) > 0 ? 0 : 4;

			switch (quadrant)
			{
			case 0: diagonal = forward + side;
				diagonal.Normalize();
				mAngle = FVector2D::DotProduct(dir, diagonal) > 0 ? 180 : 135;
				break;
			case 1: diagonal = forward - side;
				diagonal.Normalize();
				mAngle = FVector2D::DotProduct(dir, diagonal) > 0 ? 225 : 270;
				break;
			case 2: diagonal = forward - side;
				diagonal.Normalize();
				mAngle = FVector2D::DotProduct(dir, diagonal) > 0 ? 90 : 45;
				break;
			case 3: diagonal = forward + side;
				diagonal.Normalize();
				mAngle = FVector2D::DotProduct(dir, diagonal) > 0 ? 315 : 0;
				break;
			}

			mAngle = -mAngle;

			float dist = FVector::DistSquared(mEnemy->GetActorLocation(), player->GetActorLocation());
			if (dist < (mBlinkDistance*mBlinkDistance))
			{
				mBlinkRate += sign * DeltaTime;
				if (mBlinkRate < 0.f)
				{
					mBlinkVisible = true;
					sign = 1.f;
				}
				else if (mBlinkRate > mBlinkTimer)
				{
					mBlinkVisible = false;
					sign = -1.f;
				}
			}
			else
			{
				mBlinkVisible = true;
				mBlinkTimer = mBlinkRate;
			}
		
	}

}


