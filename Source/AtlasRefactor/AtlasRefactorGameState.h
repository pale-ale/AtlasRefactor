// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "AtlasRefactorGameState.generated.h"

USTRUCT(Blueprintable)
struct FTeamData{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FName Name;
	UPROPERTY(EditAnywhere)
	int32 Score;
	UPROPERTY(EditAnywhere)
	FColor Color;
};


UCLASS()
class ATLASREFACTOR_API AAtlasRefactorGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TArray<FTeamData> TeamData;
};
