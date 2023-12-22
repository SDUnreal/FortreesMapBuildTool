// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include"MarchingChunk.h"

#include "GameFramework/Actor.h"
#include "ChunkWorld.generated.h"

class MarchingChunk;

UCLASS()
class AChunkWorld : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkWorld();

	UPROPERTY(EditAnywhere, Category = "ChunkWorld")
	TSubclassOf<AMarchingChunk> Chunk;

	UPROPERTY(EditAnywhere, Category = "ChunkWorld")
	int DrawDistance = 5;

	UPROPERTY(EditAnywhere, Category = "ChunkWorld")
	int ChunkSize = 64;

	UPROPERTY(EDItAnywhere, Category = "ChunkWorld")
	int CubeSize = 100;

	UPROPERTY(EDItAnywhere, Category = "ChunkWorld")
	float zPosition = 2000.0f;

	UPROPERTY(EDItAnywhere, BluePrintReadWrite, Category = "ChunkWorld")
	FVector TargetVertex = { 0,0,0 };

	TArray<float> voxels;
	TArray<AMarchingChunk*> chunks;

	int targetVertexColorCount = 1;
	int debugDrawCount = 0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:

	UFUNCTION(BlueprintCallable, Category = "ChunkWorld")
	void DrawVertex(float LifeTime);

	void SetVoxels(const TArray<float>& Voxels);

	int GetVoxelIndex(int x, int y, int z) const;

};
