#pragma once

#include "CoreMinimal.h"
#include "VoxelDataSturct.generated.h"

USTRUCT(Atomic,BlueprintType)
struct FVoxelDataSturct
{
	
    GENERATED_USTRUCT_BODY()

public:
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MapName;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    int32 TestNum;
    
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TArray<int32> voxels;

};