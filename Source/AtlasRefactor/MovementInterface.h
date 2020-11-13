#pragma once

#include "MovementInterface.generated.h"

UINTERFACE(Blueprintable, BlueprintType)
class UMovementInterface : public UInterface
{
    GENERATED_BODY()
};

class IMovementInterface
{    
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
 	void GetNextMovementTile(FIntPoint& NextGridPosition);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
 	void MoveBlocked();
};