// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "MyLevelScriptActor.h"



AMyLevelScriptActor::AMyLevelScriptActor()
{
	mSpawnPointsSet1.Reserve(6);
	mSpawnPointsSet1.Push(FVector(-1910.0, 690.0, 1184.0));
	mSpawnPointsSet1.Push(FVector(-1670.0, -50.0, 1184.0));
	mSpawnPointsSet1.Push(FVector(30.0, 300.0, 1184.0));
	mSpawnPointsSet1.Push(FVector(30.0, 1010.0, 1184.0));
	mSpawnPointsSet1.Push(FVector(-530.0, 1470.0, 1184.0));
	mSpawnPointsSet1.Push(FVector(-1410.0, 1470.0, 1184.0));
}