// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel/ChunkWorld.h"

#include "Misc/FileHelper.h"
#include "JsonObjectConverter.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"


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
	directory = TEXT("");
	IsSelect = false;
	SetVoxels(voxels);
}

// Called every frame
void AChunkWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChunkWorld::BuildWorld()
{
	int i = 0;
	chunks[i] = GetWorld()->SpawnActor<class AMarchingChunk>(Chunk, FVector(0, 0, zPosition), FRotator::ZeroRotator);
	chunks[i]->SetChunkSize(ChunkSize);
	chunks[i]->SetCubeSize(CubeSize);
	chunks[i]->SetVoxels(voxels);
	chunks[i]->GenerateTerrian();

	//맵 빌딩 툴 특성상 다수의 청크가 필요하지 않아 주석처리했습니다.
	/*

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
	*/
}

void AChunkWorld::DrawVertex(float LifeTime)
{
	FColor color;
	float pointSize = 10.0f;
	FVector position;
	FlushPersistentDebugLines(GetWorld());

	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				position = FVector(x * CubeSize, y * CubeSize, z * CubeSize);

				if (TargetVertex == FVector(x, y, z) && targetVertexColorCount == 1)
					color = FColor::Red;
				else if (voxels[GetVoxelIndex(x, y, z)] == -1)
					color = FColor::Blue;
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

void AChunkWorld::SaveMaps()
{
// TODO : 현재 맵을 세이브 하게 하는 기능 -> 

	IsSelect = false;
	while (IsSelect == false)
	{
		this->LoadExplorer(directory, IsSelect);
	}
	FString JsonString = TEXT("HEllo");
	//directory.Append(TEXT("/save.json"));
	FString Filename = TEXT("sav");
	//FJsonObjectConverter::UStructToJsonObjectString(voxels, JsonString);
	FFileHelper::SaveStringToFile(*JsonString, *directory );
	directory = TEXT("");
}


void AChunkWorld::LoadMaps(FString path)
{
	// path에서 로드할 수 있게 해줌 
}

void AChunkWorld::LoadExplorer(FString& Directory, bool& bIsSelect)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	FString DefaultFile = TEXT("");
	FString DefaultPath = TEXT("/.");
	FString DirectoryName = TEXT("");

	if (DesktopPlatform) {
		DesktopPlatform->OpenDirectoryDialog(
			NULL,
			TEXT("select save path"),	
			DefaultPath, Directory);
	}
	if (DirectoryName.Equals(FPaths::GetBaseFilename(Directory))) bIsSelect = false;
	else bIsSelect = true;
}

void AChunkWorld::ToggleTargetVertex(FVector point)
{
// 현재 위치에 있는 토글버텍스를 반전해주는 느낌.
	voxels[GetVoxelIndex(point.X, point.Y, point.Z)] *= -1;
}

FVector AChunkWorld::FindClosestVertex(FVector point)
{
	FVector result;
	result.X = round(point.X / 200.0) * 200;
	result.Y = round(point.Y / 200.0) * 200;
	result.Z = round(point.Z / 200.0) * 200;

	return result;
}
