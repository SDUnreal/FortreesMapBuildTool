// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkWorld.generated.h"

UCLASS()
class AChunkWorld : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkWorld();

	UPROPERTY(EditAnywhere, Category = "ChunkWorld")
	TSubclassOf<AActor> Chunk;

	UPROPERTY(EditAnywhere, Category = "ChunkWorld")
	int DrawDistance = 5;

	UPROPERTY(EditAnywhere, Category = "ChunkWorld")
	int ChunkSize = 32;

	UPROPERTY(EDItAnywhere, Category = "ChunkWorld")
	int CubeSize = 0;

	UPROPERTY(EDItAnywhere, Category = "ChunkWorld")
	float zPosition = 0.0f;

	int debugDrawCount = 0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Voxel Visualization")
	void CallMarchingChunkDrawVertex();

};
