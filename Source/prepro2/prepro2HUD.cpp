// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "prepro2.h"
#include "prepro2HUD.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"

Aprepro2HUD::Aprepro2HUD()
{
	
	static ConstructorHelpers::FObjectFinder<UFont> mFontObj(TEXT("/Game/Font/Roboto"));
	if (mFontObj.Succeeded())
	{
		mFont = mFontObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> EButtonPromptObj(TEXT("/Game/FirstPerson/Textures/item_02"));
	if (EButtonPromptObj.Succeeded())
	{
		EButtonPrompt = EButtonPromptObj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> EButtonPromptFirstObj(TEXT("/Game/FirstPerson/Textures/item_first"));
	if (EButtonPromptFirstObj.Succeeded())
	{
		EButtonPromptFirst = EButtonPromptFirstObj.Object;
	}


}

void Aprepro2HUD::BeginPlay()
{
	
}

void Aprepro2HUD::DrawHUD()
{
	Super::DrawHUD();

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// draw the crosshair
	FCanvasTileItem TileItem( Center, EButtonPrompt->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;


	Aprepro2Character* player = Cast<Aprepro2Character>(UGameplayStatics::GetPlayerPawn(this, 0));
	

	// TODO: get input for from player controller rather than hardcoding y
	if (player->IsPickUpTriggerActivated())
	{
		TileItem.Texture = EButtonPrompt->Resource;//EButtonPrompt->Resource;
		if (FirstPickup)
		{
			TileItem.Texture = EButtonPromptFirst->Resource;
		}
		float ratioY = EButtonPrompt->GetSurfaceWidth() / EButtonPrompt->GetSurfaceHeight();
		FVector2D canvasSize = FVector2D(Canvas->ClipY * ratioY, Canvas->ClipY);
		TileItem.Size = canvasSize; 
		TileItem.Position = FVector2D(Canvas->ClipX * 0.05, 0);
		Canvas->DrawItem(TileItem);
		//Canvas->SetDrawColor(255, 0, 0);
		//Canvas->DrawText(mFont, "Press E to Pick Up Item", Canvas->ClipX * 0.5f - 175.f, Canvas->ClipY - 100.f, 2.f,2.f);
	}

	if (DisplayPopup)
	{
		TileItem.Texture = GivenTexture->Resource;
		float ratioY = GivenTexture->GetSurfaceWidth() / GivenTexture->GetSurfaceHeight();
		FVector2D canvasSize = FVector2D(Canvas->ClipY * ratioY, Canvas->ClipY);
		TileItem.Size = canvasSize;
		TileItem.Position = FVector2D(Canvas->ClipX * 0.05, 0);
		Canvas->DrawItem(TileItem);
	}

				/*float angle = FMath::Acos(FVector2D::DotProduct(dir,forward));
				FVector2D compassDir(0, -1);

				angle = FVector2D::CrossProduct(dir, forward) > 0 ? 2 * 3.141592 - angle : angle;

				float S, C;
				FMath::SinCos(&S, &C, angle);

				compassDir = FVector2D((C * compassDir.X - S * compassDir.Y), S * compassDir.X + C * compassDir.Y);
				compassDir.Normalize();*/
}
   
