// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "MyLevelScriptActor.h"



AMyLevelScriptActor::AMyLevelScriptActor()
{
	mPatrolPointsSet1.Reserve(2);

	mPatrolPointsSet1.Push(FVector(-3440.000000, -2830.000000, 1935.000000));
	mPatrolPointsSet1.Push(FVector(115.000000, -4255.000000, 1935.000000));

	mPatrolPointsSet2.Reserve(6);


	mPatrolPointsSet2.Push(FVector(550.000000, -2945.000000, 1935.000000));
	mPatrolPointsSet2.Push(FVector(-1130.000000, -405.000000, 1935.000000));
	mPatrolPointsSet2.Push(FVector(-520.000000, 110.000000, 1935.000000));
	mPatrolPointsSet2.Push(FVector(-70.000000, -300.000000, 1935.000000));
	mPatrolPointsSet2.Push(FVector(-205.000000, -1050.000000, 1935.000000));
	mPatrolPointsSet2.Push(FVector(-775.000000, -1050.000000, 1935.000000));

	mPatrolPointsSet3.Reserve(7);

	mPatrolPointsSet3.Push(FVector(-1745.000000, -30.000000, 1935.000000));
	mPatrolPointsSet3.Push(FVector(-1130.000000, -405.000000, 1935.000000));
	mPatrolPointsSet3.Push(FVector(-520.000000, 110.000000, 1935.000000));
	mPatrolPointsSet3.Push(FVector(-70.000000, -300.000000, 1935.000000));
	mPatrolPointsSet3.Push(FVector(-205.000000, -1050.000000, 1935.000000));
	mPatrolPointsSet3.Push(FVector(-775.000000, -1050.000000, 1935.000000));
	mPatrolPointsSet3.Push(FVector(-3200.000000, -555.000000, 1935.000000));

	mPatrolPointsSet4.Reserve(5);

	mPatrolPointsSet4.Push(FVector(-5650.000000, 2395.000000, 1942.634644));
	mPatrolPointsSet4.Push(FVector(-5190.000000, 4130.000000, 1942.634644));
	mPatrolPointsSet4.Push(FVector(-5820.000000, 4705.000000, 1942.634644));
	mPatrolPointsSet4.Push(FVector(-6280.000000, 4260.000000, 1942.634644));
	mPatrolPointsSet4.Push(FVector(-6105.000000, 3670.000000, 1942.634644));

	mEnemies.Reserve(4);
}

void AMyLevelScriptActor::FirstEnemyDeathEvent()
{
	mEnemies[1]->isAlive = true;
}

