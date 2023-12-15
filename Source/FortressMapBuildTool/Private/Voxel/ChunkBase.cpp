// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel/ChunkBase.h"

#include "../Util/Enums.h"
#include "ProceduralMeshComponent.h"


// Sets default values
AChunkBase::AChunkBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<UProceduralMeshComponent>("mesh");

	mesh->SetCastShadow(false);
	SetRootComponent(mesh);
}

// Called when the game starts or when spawned
void AChunkBase::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateHeightMap();
	GenerateMesh();

	UE_LOG(LogTemp, Log, TEXT("VertexCount : %d"), vertexCount);

	ApplyMesh();
}

void AChunkBase::GenerateHeightMap()
{
}

void AChunkBase::GenerateMesh()
{
}

void AChunkBase::ApplyMesh() const
{
	mesh->CreateMeshSection(0, meshData.Vertices, meshData.Triangles, meshData.Normals, meshData.UVData, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}

