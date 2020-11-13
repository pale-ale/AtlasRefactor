// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitBase.h"
#include "DrawDebugHelpers.h"
#include "AbilityComponent.h"
#include "RefactorController.h"

#include "GridManager.h"


// Sets default values
AUnitBase::AUnitBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UnitMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Unit Mesh"));
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("Ability Component"));
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	WidgetComponent->AttachTo(UnitMeshComponent);
	SetRootComponent(UnitMeshComponent);
}

// Called when the game starts or when spawned
void AUnitBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AUnitBase::GetMoveCost()
{
	auto startpoint = gridManager->IndexToXY(gridIndex);
	int32 totalMovementCost = 0;
	FIntPoint endpoint;
	for (int i=0; i<MovementWaypoints.Num(); i++){
		int32 waypointCost = 0;
		endpoint = MovementWaypoints[i];
		gridManager->GetPathStartEnd(startpoint, endpoint, waypointCost);
		totalMovementCost += waypointCost;
		startpoint = endpoint;
	}
	return totalMovementCost;
}

TArray<FIntPoint> AUnitBase::GetMovementTiles(){
	auto tilesWePathThrough = TArray<FIntPoint>();
	auto startpoint = gridManager->IndexToXY(gridIndex);
	FIntPoint endpoint;
	for (int i=0; i<MovementWaypoints.Num(); i++){
		endpoint = MovementWaypoints[i];
		int32 _ = 0;
		tilesWePathThrough.Append(gridManager->GetPathStartEnd(startpoint, endpoint, _));
		startpoint = endpoint;
	}
	return tilesWePathThrough;
}


