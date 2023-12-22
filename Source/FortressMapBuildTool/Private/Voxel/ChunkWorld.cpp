// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel/ChunkWorld.h"

// Sets default values
AChunkWorld::AChunkWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AChunkWorld::BeginPlay()
{
	voxels.Init(0, (ChunkSize + 1) * (ChunkSize + 1) * (ChunkSize + 1));
	chunks.Init(nullptr, (2 * DrawDistance) * (2 * DrawDistance));

	Super::BeginPlay();
	int i = 0;
	SetVoxels(voxels);
	for (int x = -DrawDistance; x < DrawDistance; x++)
	{
		for (int y = -DrawDistance; y < DrawDistance; y++)
		{
			chunks[i] = GetWorld()->SpawnActor<class AMarchingChunk>(Chunk, FVector(x * ChunkSize * CubeSize, y * ChunkSize * CubeSize, zPosition), FRotator::ZeroRotator);
			chunks[i]->SetChunkSize(ChunkSize);
			chunks[i]->SetCubeSize(CubeSize);
			chunks[i]->SetVoxels(voxels);
			chunks[i]->GenerateTerrian();
			i++;
		}
	}
}

// Called every frame
void AChunkWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChunkWorld::DrawVertex(float LifeTime)
{
	FColor color;
	float pointSize = 10.0f;

	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				FVector position = FVector(x * CubeSize, y * CubeSize, z * CubeSize);

				if (TargetVertex == FVector(x, y, z) && targetVertexColorCount == 1)
					color = FColor::Blue;
				else if (voxels[GetVoxelIndex(x, y, z)] == -1)
					color = FColor::Red;
				else
					color = FColor::Green;

				DrawDebugPoint(GetWorld(), position, pointSize, color, true, LifeTime);
			}
		}
	}

	targetVertexColorCount *= -1;
}

void AChunkWorld::SetVoxels(const TArray<float>& Voxels)
{
	const auto position = GetActorLocation() / 100;

	for (int x = 0; x <= ChunkSize; x++)
	{
		for (int y = 0; y <= ChunkSize; y++)
		{
			for (int z = 0; z <= ChunkSize; z++)
			{
				//voxels[GetVoxelIndex(x, y, z)] = noise->GetNoise(x + position.X, y + position.Y, z + position.Z);
				if (x == 0 || x == ChunkSize - 1 || y == 0 || y == ChunkSize - 1 || z == 0 || z == ChunkSize - 1)
				{
					// 외곽의 점들
					voxels[GetVoxelIndex(x, y, z)] = 1.0f;
				}
				else
				{
					// 내부의 점들
					voxels[GetVoxelIndex(x, y, z)] = -1.0f;
				}

				
			}
		}
	}
	//voxels = Voxels;
}

int AChunkWorld::GetVoxelIndex(int x, int y, int z) const
{
	return z * (ChunkSize + 1) * (ChunkSize + 1) + y * (ChunkSize + 1) + x;
}