// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Util/VoxelDataSturct.h"
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

	UPROPERTY(EditAnywhere, Category = "ChunkWorld")
	int CubeSize = 100;

	UPROPERTY(EditAnywhere, Category = "ChunkWorld")
	float zPosition = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChunkWorld")
	FVector TargetVertex = { 0,0,0 };

	TArray<float> voxels; // �����ؾ��ϴ� ������ -> txt�� �׿�.. �𸮾󿡼� ã�ƺ��Կ�
	TArray<AMarchingChunk*> chunks;
	FVoxelDataSturct data;



	
	// ����Ž���� ���� �����͵�
	FString directory;
	bool IsSelect;

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
	void BuildWorld();

	UFUNCTION(BlueprintCallable, Category = "ChunkWorld")
	void DrawVertex(float LifeTime);

	void SetVoxels(const TArray<float>& Voxels);

	int GetVoxelIndex(int x, int y, int z) const;

	UFUNCTION(BlueprintCallable, Category = "editor")
	void SaveMaps();

	UFUNCTION(BlueprintCallable, Category = "editor")
	void LoadMaps(FString path);

	UFUNCTION(BlueprintCallable, Category = "editor")
	void LoadExplorer(FString& Directory, bool& bIsSelect);

	
	UFUNCTION(BlueprintCallable, Category = "editor")
	void ToggleTargetVertex(FVector point);

	UFUNCTION(BlueprintCallable, Category = "editor")
	FVector FindClosestVertex(FVector point);

};

// UI -> ��Ÿ�ӿ��� �����°� �������? ��� ���� UI�� ���� ��������

// Enhanced Input
// target vertex �����̴�
// rf : �̵�? bool 
// space bar : toggle bool  
// ���� �ҷ������ -> ���콺�� UI Ŭ���ؼ� ����ϰų� bool
// ī�޶� ����?  wasd camera���� �������浹 -> �׽�Ʈ �ѹ� �غ��� -> �ٸ� ��� ������� �������� ��������Ҷ� ����.. ���⼭ ���� ��������Ű����ؿ�
// gizmo ���׶��� ���콺��  ī�޶󿡼� 



