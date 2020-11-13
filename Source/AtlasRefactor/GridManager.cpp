// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "GridManager.h"

void UGridManager::ShowDebugGrid()
{
    for (int x = 0; x <= gridCountX; x++)
    {
        FVector start = *new FVector(squareSize * x, 0, z);
        FVector end = *new FVector(squareSize * x, squareSize * gridCountY, z);
        DrawDebugLine(GetWorld(), start, end, FColorList::Green, false);
    }
    for (int y = 0; y <= gridCountY; y++)
    {
        FVector start = *new FVector(0, squareSize * y, z);
        FVector end = *new FVector(squareSize * gridCountX, squareSize * y, z);
        DrawDebugLine(GetWorld(), start, end, FColorList::Green, false);
    }
}

bool UGridManager::AddUnit(AUnitBase *newUnit, FIntPoint GridPosition)
{
    if (newUnit)
    {
        grid.Insert(XYToIndex(GridPosition), newUnit);
        newUnit->SetActorLocation(GridToWorld(GridPosition));
        newUnit->gridManager = this;
        newUnit->gridIndex = XYToIndex(GridPosition);
    }
    return true;
}

FVector UGridManager::GridToWorld(int32 index)
{
    auto gridPoint = IndexToXY(index);
    return *new FVector((gridPoint.X + .5f) * squareSize, (gridPoint.Y + .5f) * squareSize, z);
}

FVector UGridManager::GetCenter()
{
    return *new FVector((gridCountX / 2 + .5f) * squareSize, (gridCountY / 2 + .5f) * squareSize, z);
}

FVector UGridManager::GridToWorld(FIntPoint GridPosition)
{
    return *new FVector((GridPosition.X + .5f) * squareSize, (GridPosition.Y + .5f) * squareSize, z);
}

void UGridManager::MoveUnit(AUnitBase *unit, FIntPoint GridPosition, bool& bWasUnitMoved)
{
    int32 gridIndex = XYToIndex(GridPosition);
    if (grid.IsValidIndex(gridIndex) && grid[gridIndex]->IsValidLowLevel()){
        unit->Execute_MoveBlocked(unit);
        bWasUnitMoved = false;
        return;
    }
    
    grid.RemoveAt(IndexFromUnit(unit));
    unit->gridIndex = XYToIndex(GridPosition);
    grid.Insert(unit->gridIndex, unit);
    unit->SetActorLocation(GridToWorld(GridPosition));
    bWasUnitMoved = true;
}

void UGridManager::bDoesMovementOverlap(FIntPoint GridPosition, bool& bOverlaps){
    TArray<AUnitBase*> units = TArray<AUnitBase*>();
    AUnitBase * unit;
    int overlapCount = 0;
    GetAllUnits(units);
    for (int i = 0; i < units.Num(); i++){
        unit = units[i];
        //if(unit->Execute_GetNextMovementTile(unit))
    }
}


TArray<FIntVector> UGridManager::StartDisplayMovementRange(AUnitBase *unit)
{
    auto intvs = TArray<FIntVector>();
    if ( unit ){
        int32 unitIndex = IndexFromUnit(unit);
        auto possibleMoveDestinaitons = GetIndicesInRange(unitIndex, unit->movementRange);
        for (int i = 0; i < possibleMoveDestinaitons.Num(); i++)
        {
            int x = IndexToXY(possibleMoveDestinaitons[i]).X;
            int y = IndexToXY(possibleMoveDestinaitons[i]).Y;
            intvs.Add(FIntVector(x, y, 0));
        }
    }else{
        UE_LOG(LogTemp, Warning, TEXT("unit invalid"));
    }
    return intvs;
}

TArray<FIntVector> UGridManager::StartDisplayRange(FIntPoint start, int32 range)
{
    int32 startIndex = XYToIndex(start);
    auto intvs = TArray<FIntVector>();
    auto possibleMoveDestinaitons = GetIndicesInRange(startIndex, range);
    for (int i = 0; i < possibleMoveDestinaitons.Num(); i++)
    {
        int x = IndexToXY(possibleMoveDestinaitons[i]).X;
        int y = IndexToXY(possibleMoveDestinaitons[i]).Y;
        intvs.Add(FIntVector(x, y, 0));
    }
    return intvs;
}

void UGridManager::StopDisplayMovementRange()
{
    FlushPersistentDebugLines(GetWorld());
}

TArray<FIntPoint> UGridManager::GetPathUnitEnd(AUnitBase *unit, FIntPoint end, int32& cost)
{
    TArray<int32>* indicesOnPath = new TArray<int32>();
    auto coordsOnPath = TArray<FIntPoint>();
    cost = GetIndicesOnPath(
        IndexFromUnit(unit),
        XYToIndex(end),
        GetIndicesInRange(IndexFromUnit(unit), unit->movementRange),
        *indicesOnPath);
    for (int i = 0; i < indicesOnPath->Num(); i++)
    {
        coordsOnPath.Add(IndexToXY((*indicesOnPath)[i]));
    }
    return coordsOnPath;
}

TArray<FIntPoint> UGridManager::GetPathStartEnd(FIntPoint start, FIntPoint end, int32& cost)
{
    TArray<int32>* indicesOnPath = new TArray<int32>();
    auto coordsOnPath = TArray<FIntPoint>();
    cost = GetIndicesOnPath(
        XYToIndex(start),
        XYToIndex(end),
        TArray<int32>(),
        *indicesOnPath);
    for (int i = 0; i < indicesOnPath->Num(); i++)
    {    
        coordsOnPath.Add(IndexToXY((*indicesOnPath)[i]));
    }
    return coordsOnPath;
}

TArray<FIntPoint> UGridManager::GetPathVertices(TArray<FIntPoint> wayPoints, int32& cost){
    TArray<FIntPoint> pointsOnPath = TArray<FIntPoint>();
    TArray<FIntPoint> partialPath = TArray<FIntPoint>();
    int32 subCost;
    if (wayPoints.Num() > 0) pointsOnPath.Add(wayPoints[0]);
    for (int i = 0; i < wayPoints.Num() - 1; i++){
        subCost = 0;
        partialPath = GetPathStartEnd(wayPoints[i], wayPoints[i+1], subCost);
        partialPath.RemoveAt(0);
        pointsOnPath.Append(partialPath);
        cost += subCost;
    }

    return pointsOnPath;
}

void UGridManager::GetAllUnits(TArray<AUnitBase *>& unitRefs){
    for (int i = 0; i < grid.GetMaxIndex(); i++){
        if (grid.IsAllocated(i)){
            unitRefs.Add(grid[i]);
        }
    }
}

void UGridManager::ResolveTurn(){
    // THIS IS UNUSED BECAUSE IDK HOW TO USE TIMING / "DELAYS" IN C++ 
    //UE_LOG(LogTemp, Warning, TEXT("\"Is there no reverse path?\" -Gridmanager.cpp"));
    for (uint8 turnStateID = 0; turnStateID < (uint8)ETurnState::LAST; turnStateID++){
        CurrentResolvingTurnState = static_cast<ETurnState>(turnStateID);
        TArray<AUnitBase *> unitsToResolve = TArray<AUnitBase *>();

        GetAllUnits(unitsToResolve);
        UE_LOG(LogTemp, Warning, TEXT("Resolving: %d"), turnStateID);
        AUnitBase * resolvingUnit;
        for (int i = 0; i < unitsToResolve.Num(); i++){
            resolvingUnit = unitsToResolve[i];
            UE_LOG(LogTemp, Warning, TEXT("resolving unit on gridindex %d"), resolvingUnit->gridIndex);
            resolvingUnit->AbilityComponent->Execute_Resolve(resolvingUnit->AbilityComponent, CurrentResolvingTurnState);
        }
    }
}

TArray<int32> UGridManager::GetSurroundingIndices(int32 index)
{    
    auto surroundingIndices = TArray<int32>();
    for (int32 i = 0; i < 8; i++)
    {
        auto xy = IndexToXY(index);
        int testX = xy.X + dx[i];
        int testY = xy.Y + dy[i];
        int testIndex = XYToIndex(FIntPoint(testX, testY));
        if (testX >= 0 && testX < gridCountX && testY >= 0 && testY < gridCountY)
        {
            surroundingIndices.Add(testIndex);
        }
    }
    return surroundingIndices;
}

TArray<int32> UGridManager::GetSurroundingCardinals(int32 index)
{
    auto surroundingCardinals = TArray<int32>();
    for (int32 i = 1; i < 8; i += 2)
    {
        auto xy = IndexToXY(index);
        int testX = xy.X + dx[i];
        int testY = xy.Y + dy[i];
        int testIndex = XYToIndex(FIntPoint(testX, testY));
        if (testX >= 0 && testX < gridCountX && testY >= 0 && testY < gridCountY)
        {
            surroundingCardinals.Add(testIndex);
        }
    }
    return surroundingCardinals;
}

TArray<int32> UGridManager::GetSurroundingOrdinals(int32 index)
{
    auto surroundingOrdinals = TArray<int32>();
    for (int32 i = 0; i < 8; i += 2)
    {
        auto xy = IndexToXY(index);
        int testX = xy.X + dx[i];
        int testY = xy.Y + dy[i];
        int testIndex = XYToIndex(FIntPoint(testX, testY));
        if (testX >= 0 && testX < gridCountX && testY >= 0 && testY < gridCountY)
        {
            surroundingOrdinals.Add(testIndex);
        }
    }
    return surroundingOrdinals;
}

TArray<int32> UGridManager::GetIndicesInRange(int32 index, int32 range)
{
    auto newIndicesInRange = TArray<int32>();

    auto openIndices = TArray<int32>();
    auto openCosts = TArray<int32>();

    auto closedIndices = TArray<int32>();
    auto closedCosts = TArray<int32>();

    openIndices.Add(index);
    openCosts.Add(0);

    while (openIndices.Num() > 0)
    {
        int32 currentCost = FMath::Min(openCosts);
        int32 currentIndicesIndex = openCosts.Find(currentCost);
        if (currentIndicesIndex < 0)
        {
            break;
        }
        int32 currentIndex = openIndices[currentIndicesIndex];
        openIndices.RemoveAt(currentIndicesIndex);
        openCosts.RemoveAt(currentIndicesIndex);

        closedIndices.Add(currentIndex);
        closedCosts.Add(currentCost);

        
        TArray<int32> neighbors = GetSurroundingIndices(currentIndex);
        for (int j = 0; j < neighbors.Num(); j++)
        {
            int32 neighbor = neighbors[j];
            if (closedIndices.Contains(neighbor))
            {
                continue;
            }
            int32 calculatedNeighborCost = currentCost + AStarGetHCost(currentIndex, neighbor);
            if (!openIndices.Contains(neighbor) && calculatedNeighborCost <= range)
            {
                openIndices.Add(neighbor);
                newIndicesInRange.Add(neighbor);
                openCosts.Add(calculatedNeighborCost);
            }
            else
            {
                int32 neighborIndex = openIndices.Find(neighbor);
                if (neighborIndex >= 0 && calculatedNeighborCost < openCosts[neighborIndex])
                {
                    openCosts[neighborIndex] = calculatedNeighborCost;
                }
            }
        }
    }
    return newIndicesInRange;
}

int32 UGridManager::GetIndicesOnPath(int32 startIndex, int32 endIndex, TArray<int32> traversibleIndices, TArray<int32>& indicesOnPath)
{
    int32 endFCost = -1;
    auto openIndices = TArray<int32>();
    auto openFCosts = TArray<int32>(); //gcost to start, hcost to end, fcost =g+hcost
    auto openParentIndices = TArray<int32>();

    auto closedIndices = TArray<int32>();
    auto closedFCosts = TArray<int32>();
    auto closedParentIndices = TArray<int32>();

    openIndices.Add(startIndex);
    if (traversibleIndices.Num() > 0){ traversibleIndices.Add(startIndex);}
    openFCosts.Add(AStarGetHCost(startIndex, endIndex));
    openParentIndices.Add(-1);
    int32 currentIndex = -1;
    while (true)
    {
        int32 currentFCost = FMath::Min(openFCosts);
        int32 currentIndicesIndex = openFCosts.Find(currentFCost);
        if (currentIndicesIndex < 0)
        {
            break;
        }
        currentIndex = openIndices[currentIndicesIndex];
       
        int32 currentParent = openParentIndices[currentIndicesIndex];
        openIndices.RemoveAt(currentIndicesIndex);
        openFCosts.RemoveAt(currentIndicesIndex);
        openParentIndices.RemoveAt(currentIndicesIndex);

        closedIndices.Add(currentIndex);
        closedFCosts.Add(currentFCost);
        closedParentIndices.Add(currentParent);
        
        if (currentIndex == endIndex){
            endFCost = currentFCost - closedFCosts[0];
            break;
        }

        TArray<int32> neighbors = GetSurroundingIndices(currentIndex);
        for (int j = 0; j < neighbors.Num(); j++)
        {
            int32 neighbor = neighbors[j];
            if (closedIndices.Contains(neighbor) || ( traversibleIndices.Num() != 0 && traversibleIndices.Contains(neighbor)))
            {
                continue;
            }
            int32 calculatedNeighborFCost = currentFCost + AStarGetHCost(currentIndex, neighbor);
            if (!openIndices.Contains(neighbor))
            {
                openIndices.Add(neighbor);
                openFCosts.Add(calculatedNeighborFCost);
                openParentIndices.Add(currentIndex);
            }
            else
            {
                int32 neighborIndex = openIndices.Find(neighbor);
                if (calculatedNeighborFCost < openFCosts[neighborIndex])
                {
                    openFCosts[neighborIndex] = calculatedNeighborFCost;
                    openParentIndices[neighborIndex] = currentIndex;
                }
            }
        }
    }


    int32 reverseCurrentIndicesIndex = closedIndices.Num() - 1;
    for (int i = 0; i < 50; i++)
    {
        int32 reverseCurrentIndex = closedIndices[reverseCurrentIndicesIndex];
        int32 parentIndex = closedParentIndices[reverseCurrentIndicesIndex];
        indicesOnPath.Insert(reverseCurrentIndex, 0);
        if (parentIndex == -1)
        {
            return endFCost;
        }
        reverseCurrentIndicesIndex = closedIndices.Find(parentIndex);
    }
    UE_LOG(LogTemp, Warning, TEXT("\"Is there no reverse path?\" -Gridmanager.cpp"));
    return endFCost;
}

int32 UGridManager::AStarGetHCost(int32 index, int32 endIndex)
{
    auto endPoint = IndexToXY(endIndex);
    auto currentPoint = IndexToXY(index);
    int32 dxendcurrent = endPoint.X - currentPoint.X;
    int32 dyendcurrent = endPoint.Y - currentPoint.Y;
    int32 ordinalMoves = FMath::Min(FMath::Abs(dxendcurrent), FMath::Abs(dyendcurrent));
    int32 cardinalMoves = FMath::Max(FMath::Abs(dxendcurrent), FMath::Abs(dyendcurrent)) - ordinalMoves;

    return ordinalMoves * FMath::RoundToInt(ordinalCost * 10) + cardinalMoves * FMath::RoundToInt(cardinalCost * 10);
}

void UGridManager::LetUnitMove(AUnitBase *unit){
    UE_LOG(LogTemp, Warning, TEXT("would let unit move"));
}
