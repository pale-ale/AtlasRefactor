#pragma once

UENUM(BlueprintType)
enum class ETurnState : uint8
{
    Prep  UMETA(DisplayName="Prep"),
    Dash  UMETA(DisplayName="Dash"),
    Burst UMETA(DisplayName="Burst"),
    Move  UMETA(DisplayName="Move"),
    LAST  UMETA(Hidden)
};