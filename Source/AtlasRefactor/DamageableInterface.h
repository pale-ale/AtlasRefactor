#pragma once

#include "DamageableInterface.generated.h"

UINTERFACE(Blueprintable, BlueprintType)
class UDamageableInterface : public UInterface
{
    GENERATED_BODY()
};

class IDamageableInterface
{    
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Damage Events")
    void takeDamage(int32 amount);
};