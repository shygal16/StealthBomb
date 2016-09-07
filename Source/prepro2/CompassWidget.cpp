// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "CompassWidget.h"
#include "prepro2Character.h"
#include "Enemy_RealTest.h"

void UCompassWidget::GetQuadrant()
{
	Aprepro2Character* player = Cast<Aprepro2Character>(UGameplayStatics::GetPlayerPawn(this, 0));
	//mEnemyHeard = false;
	if (mEnemy && mCompassState == CompassState::Visible)
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
			}
		}
	}

}



