// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../Util/ChunkMeshData.h"

#include "GameFramework/Actor.h"
#include "ChunkBase.generated.h"

class UProceduralMeshComponent;

UCLASS(ABSTRACT)
class AChunkBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkBase();

	UPROPERTY(EditDefaultsOnly, Category = "Chunk")
	int chunkSize = 64;
	UPROPERTY(EditDefaultsOnly, Category = "Chunk")
	int cubeSize = 100;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GenerateHeightMap();
	virtual void GenerateMesh();

	TObjectPtr<UProceduralMeshComponent> mesh;
	FChunkMeshData meshData;
	int vertexCount = 0;

public:
	void ApplyMesh() const;
	void GenerateTerrian();

	void SetCubeSize(int CubeSize) { this->cubeSize = CubeSize; }
	void SetChunkSize(int ChunkSize) { this->chunkSize = ChunkSize; }

};
