#pragma once

#include "TurnState.h"
#include "Resolvable.generated.h"

DECLARE_DYNAMIC_DELEGATE(FunctionDelegate);

UINTERFACE(Blueprintable, BlueprintType)
class UResolvable : public UInterface
{
    GENERATED_BODY()
};

class IResolvable
{    
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Resolving")
    void Resolve(ETurnState TurnState);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Resolving")
    void GetResolveTimeSeconds(float& duration, ETurnState TurnState);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Resolving")
    void SetResolveCallback(const FunctionDelegate& Callback);
};