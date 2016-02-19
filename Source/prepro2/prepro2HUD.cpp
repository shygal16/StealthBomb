// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "prepro2.h"
#include "prepro2HUD.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"

Aprepro2HUD::Aprepro2HUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshiarTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshiarTexObj.Object;
	
	//bomb used
	static ConstructorHelpers::FObjectFinder<UTexture2D> mBombUsedObj(TEXT("/Game/FirstPerson/Textures/bomb_used"));
	if (mBombUsedObj.Succeeded())
	{
		mBombUsed = mBombUsedObj.Object;
	}

	//bomb unused
	static ConstructorHelpers::FObjectFinder<UTexture2D> mBombUnusedObj(TEXT("/Game/FirstPerson/Textures/bomb_unused"));
	if (mBombUnusedObj.Succeeded())
	{
		mBombUnused = mBombUnusedObj.Object;
	}

	//bomb selected
	static ConstructorHelpers::FObjectFinder<UTexture2D> mBombSelectedObj(TEXT("/Game/FirstPerson/Textures/bomb_selected"));
	if (mBombSelectedObj.Succeeded())
	{
		mBombSelected = mBombSelectedObj.Object;
	}

	mBombTextureWidth = mBombUsed->GetSurfaceWidth();
	
}


void Aprepro2HUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5)),
										   (Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f)) );

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );

	const FVector2D BombPosition(20.0f, (Canvas->ClipY - (mBombUsed->GetSurfaceHeight() * 0.5f) - 20.0f));

	Aprepro2Character* player = Cast<Aprepro2Character>(UGameplayStatics::GetPlayerPawn(this, 0));

	int bombIndex = player->GetBombIndex();
	uint8 maxBombs = player->GetMaxBombs();

	//draw used bombs
	for (int i = 0; i < bombIndex; ++i)
	{
		TileItem.Position = BombPosition;
		TileItem.Position.X += i*(mBombTextureWidth + 5.0f);
		if (i == player->GetBombSelected())
		{
			TileItem.Texture = mBombSelected->Resource;
		}
		else
		{

			TileItem.Texture = mBombUsed->Resource;
		}
		Canvas->DrawItem(TileItem);
	}

	//draw unused bombs
	for (int i = bombIndex; i < maxBombs; ++i)
	{
		TileItem.Position = BombPosition;
		TileItem.Position.X += i*(mBombTextureWidth + 5.0f);
		TileItem.Texture = mBombUnused->Resource;
		Canvas->DrawItem(TileItem);
	}
}

