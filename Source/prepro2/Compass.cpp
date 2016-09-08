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
}

// Called every frame
void ACompass::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	mEnemyHeard = false;
	if (mCompassState == CompassState::Visible)
	{
		GetQuadrant();
	}
	UpdateState();
}

void ACompass::UpdateState()
{
	FOutputDeviceNull ar;
	CallFunctionByNameWithArguments(TEXT("UpdateWidget"), ar, NULL, true);
}

void ACompass::GetQuadrant()
{
	Aprepro2Character* player = Cast<Aprepro2Character>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (mEnemy)
	{
		if (mEnemy->Screeched)
		{
			mEnemyHeard = true;

			FVector2D dir = (FVector2D)mEnemy->GetActorLocation() - (FVector2D)player->GetActorLocation();
			FVector2D forward = (FVector2D)player->GetFirstPersonCameraComponent()->GetForwardVector();
			FVector2D side = (FVector2D)player->GetFirstPersonCameraComponent()->GetRightVector();
			dir.Normalize();

			int quadrant = 0;

			quadrant += FVector2D::DotProduct(dir, side) < 0 ? 0 : 1;
			quadrant += FVector2D::DotProduct(dir, forward) > 0 ? 0 : 2;
		}

		else if (mEnemy->GetVelocity().DistSquaredXY > 0)
		{
			float dist = FVector::DistSquared(mEnemy->GetActorLocation(), player->GetActorLocation());
			if (dist < 5000 * 700)
			{
				mEnemyHeard = true;

				FVector2D dir = (FVector2D)mEnemy->GetActorLocation() - (FVector2D)player->GetActorLocation();
				FVector2D forward = (FVector2D)player->GetFirstPersonCameraComponent()->GetForwardVector();
				FVector2D side = (FVector2D)player->GetFirstPersonCameraComponent()->GetRightVector();
				dir.Normalize();

				int quadrant = 0;
				FVector2D diagonal;

				quadrant += FVector2D::DotProduct(dir, side) < 0 ? 0 : 1;
				quadrant += FVector2D::DotProduct(dir, forward) > 0 ? 0 : 2;
				//quadrant += FVector2D::DotProduct(dir, diagonal) > 0 ? 0 : 4;

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::FromInt(quadrant));

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
			}
		}
	}

}


