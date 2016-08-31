// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#ifndef __PREPRO2_H__
#define __PREPRO2_H__

#include "EngineMinimal.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

 struct Globals
{
public:
	Globals()
	{
		XrayOn = false;
	}
	static bool XrayOn;
}; 


#endif
