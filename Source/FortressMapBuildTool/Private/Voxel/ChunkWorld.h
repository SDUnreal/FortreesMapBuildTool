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

	TArray<float> voxels; // 저장해야하는 데이터 -> txt나 그외.. 언리얼에서 찾아볼게요
	TArray<AMarchingChunk*> chunks;
	FVoxelDataSturct data;



	
	// 파일탐색기 관련 데이터들
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

// UI -> 런타임에서 돌리는게 맞을까요? 대신 예쁜 UI는 조금 어려울수도

// Enhanced Input
// target vertex 움직이는
// rf : 이동? bool 
// space bar : toggle bool  
// 저장 불러오기는 -> 마우스로 UI 클릭해서 사용하거나 bool
// 카메라 무빙?  wasd camera에서 레이저충돌 -> 테스트 한번 해보고 -> 다른 모양 덩어리에서 도넛으로 만들려고할때 맵을.. 여기서 조금 문제생길거같긴해요
// gizmo 동그란거 마우스로  카메라에서 



