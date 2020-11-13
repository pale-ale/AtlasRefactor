// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "UObject/NoExportTypes.h"
#include "Templates/Tuple.h"
#include "Containers/SparseArray.h"
#include "Math/IntPoint.h"
#include "TurnState.h"
#include "AbilityComponent.h"
#include "MovementInterface.h"

#include "GridManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ATLASREFACTOR_API UGridManager : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Dimensions")
	int32 gridCountX = 30;
	UPROPERTY(EditDefaultsOnly, Category = "Dimensions")
	int32 gridCountY = 30;
	UPROPERTY(EditDefaultsOnly, Category = "Dimensions")
	int32 squareSize = 100;
	UPROPERTY(EditDefaultsOnly)
	int32 z = 70;
	UPROPERTY(BlueprintReadOnly, Category = "Resolving")
	ETurnState CurrentResolvingTurnState;

	UFUNCTION(BlueprintCallable)
	void ShowDebugGrid();

	UFUNCTION(BlueprintCallable)
	bool AddUnit(AUnitBase *newUnit, FIntPoint GridPosition);

	UFUNCTION(BlueprintCallable)
	FVector GridToWorld(FIntPoint GridPosition);
	FVector GridToWorld(int32 index);

	UFUNCTION(BlueprintCallable)
	void MoveUnit(AUnitBase *unit, FIntPoint GridPosition, bool& bWasUnitMoved);

	UFUNCTION(BlueprintCallable)
	void bDoesMovementOverlap(FIntPoint GridPosition, bool& bOverlaps);

	UFUNCTION(BlueprintCallable)
	void LetUnitMove(AUnitBase *unit);

	UFUNCTION(BlueprintCallable)
	void ResolveTurn();

	UFUNCTION(BlueprintCallable)
	TArray<FIntVector> StartDisplayMovementRange(AUnitBase *unit);

	UFUNCTION(BlueprintCallable)
	TArray<FIntVector> StartDisplayRange(FIntPoint start, int32 range);

	UFUNCTION(BlueprintCallable)
	void StopDisplayMovementRange();

	UFUNCTION(BlueprintCallable)
	FVector GetCenter();

	UFUNCTION(BlueprintCallable)
	TArray<FIntPoint> GetPathUnitEnd(AUnitBase *unit, FIntPoint end, int32& cost);

	UFUNCTION(BlueprintCallable)
	TArray<FIntPoint> GetPathStartEnd(FIntPoint start, FIntPoint end, int32& cost);

	UFUNCTION(BlueprintCallable)
	TArray<FIntPoint> GetPathVertices(TArray<FIntPoint> wayPoints, int32& cost);

	UFUNCTION(BlueprintCallable)
	void GetAllUnits(TArray<AUnitBase * >& unitRefs);

	

	int32 IndexFromUnit(AUnitBase *unit)
	{
		return unit->gridIndex;
	}

	int32 XYToIndex(FIntPoint gridPosition)
	{
		return gridPosition.Y * gridCountX + gridPosition.X;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FIntPoint IndexToXY(int32 index)
	{
		return FIntPoint(index % gridCountX, index / gridCountX);
	}

private:
	TArray<int32> dx = {-1, -1, -1, 0, 1, 1, 1, 0};
	TArray<int32> dy = {-1, 0, 1, 1, 1, 0, -1, -1};
	float cardinalCost = 1.0f;
	float ordinalCost = 1.5f;
	TSparseArray<AUnitBase *> grid;

	
	TArray<int32> GetSurroundingIndices(int32 index);
	TArray<int32> GetSurroundingCardinals(int32 index);
	TArray<int32> GetSurroundingOrdinals(int32 index);
	TArray<int32> GetIndicesInRange(int32 index, int32 range);
	int32 GetIndicesOnPath(int32 startIndex, int32 endIndex, TArray<int32> traversibleIndices, TArray<int32>& out);
	int32 AStarGetHCost(int32 index, int32 startIndex);
};
