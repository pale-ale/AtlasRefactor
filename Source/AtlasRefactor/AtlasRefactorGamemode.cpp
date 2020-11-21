// Fill out your copyright notice in the Description page of Project Settings.


#include "AtlasRefactorGamemode.h"

void AAtlasRefactorGamemode::BeginPlay(){
    Super::BeginPlay();
    Resolver = GetWorld()->SpawnActor<AResolver>();
    FTimerHandle handle;
    FTimerDelegate del;
    del.BindUFunction(this, "OnTimeIsUp");
    GetWorldTimerManager().SetTimer(handle, del, 1, false, 10);

    EndOfTurnTime = FDateTime::UtcNow() + FTimespan(0, 0, 10);
    GridManager = NewObject<UGridManager>();

    for(int i=0; i<InitialSpawns.Num(); i++){
        FUnitLocation ful = InitialSpawns[i];
        AUnitBase * newUnit = GetWorld()->SpawnActor<AUnitBase>(ful.Unit);
        GridManager->AddUnit(newUnit, ful.Location);
    }
}

void AAtlasRefactorGamemode::Tick(float DeltaTime){
    GridManager->ShowDebugGrid();
}

void AAtlasRefactorGamemode::GetResolveTime(ETurnState TurnState, float& ResolveTime){
    TArray<AUnitBase*> units;
    ResolveTime = 0;
    GridManager->GetAllUnits(units);
    for(int i=0; i<units.Num(); i++){
        float nextDuration;
        units[i]->AbilityComponent->Execute_GetResolveTimeSeconds(units[i]->AbilityComponent, nextDuration, TurnState);
        ResolveTime = FMath::Max(ResolveTime, nextDuration);
    }
}

void AAtlasRefactorGamemode::QueueResolvables(){
    Resolver->SetActorTickEnabled(true);
    TArray<AUnitBase*> units;
    GridManager->GetAllUnits(units);
    for(int i=0; i<units.Num(); i++){
        for(int turnStateInt=0; turnStateInt<4; turnStateInt++){
            Resolver->QueueResolvable(units[i]->AbilityComponent, (ETurnState)turnStateInt);
        }
    }
}

void AAtlasRefactorGamemode::OnTimeIsUp(){
    UE_LOG(LogTemp, Warning, TEXT("Rrrrrriiinnggg"));
    for(int i=0; i<ConnectedNetworkControllers.Num(); i++){
        ConnectedNetworkControllers[i]->EndTurnTimeReached();
    }
    QueueResolvables();
}

void AAtlasRefactorGamemode::PostLogin(APlayerController * NewPlayer){
    Super::PostLogin(NewPlayer);
    ConnectedNetworkControllers.Add(Cast<ARefactorController, APlayerController>(NewPlayer));
}

void AAtlasRefactorGamemode::OnUnitDied(AUnitBase * Unit){
}
