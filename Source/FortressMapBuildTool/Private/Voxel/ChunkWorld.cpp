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
	data.MapName = TEXT("DefaultMap");
	data.TestNum = 1;
}

// Called when the game starts or when spawned
void AChunkWorld::BeginPlay()
{
	voxels.Init(-1, (ChunkSize + 1) * (ChunkSize + 1) * (ChunkSize + 1));
	chunks.Init(nullptr, (2 * DrawDistance) * (2 * DrawDistance));

	Super::BeginPlay();
	directory = TEXT("");
	IsSelect = false;
	SetVoxels(voxels);
	data.voxels = voxels;
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

	//�� ���� �� Ư���� �ټ��� ûũ�� �ʿ����� �ʾ� �ּ�ó���߽��ϴ�.
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
	this->voxels = Voxels;
}

int AChunkWorld::GetVoxelIndex(int x, int y, int z) const
{
	return z * (ChunkSize + 1) * (ChunkSize + 1) + y * (ChunkSize + 1) + x;
}

void AChunkWorld::SaveMaps()
{
// TODO : ���� ���� ���̺� �ϰ� �ϴ� ��� -> 

	IsSelect = false;
	while (IsSelect == false)
	{
		this->LoadExplorer(directory, IsSelect);
	}
	FString JsonString;
	data.voxels = voxels;
	directory.Append(TEXT("/save.json"));
	FString Filename = TEXT("sav");
	FJsonObjectConverter::UStructToJsonObjectString(data, JsonString);
	FFileHelper::SaveStringToFile(*JsonString, *directory );
	directory = TEXT("");
}


void AChunkWorld::LoadMaps(FString path)
{
	// path���� �ε��� �� �ְ� ���� 
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
// ���� ��ġ�� �ִ� ��۹��ؽ��� �������ִ� ����.
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

<<<<<<< HEAD
void AChunkWorld::ChangeVoxelsData(int Size, FVector TargetPoint, int axis, int Data)
{
	if (Size == 1)
	{
		voxels[GetVoxelIndex(TargetPoint.X, TargetPoint.Y, TargetPoint.Z)] = Data;
	}
	else if(Size > 1)
	{
		int minX = FMath::Max(0, TargetPoint.X - Size / 2);
		int maxX = FMath::Min(ChunkSize, TargetPoint.X + Size / 2);

		int minY = FMath::Max(0, TargetPoint.Y - Size / 2);
		int maxY = FMath::Min(ChunkSize, TargetPoint.Y + Size / 2);

		int minZ = FMath::Max(0, TargetPoint.Z - Size / 2);
		int maxZ = FMath::Min(ChunkSize, TargetPoint.Z + Size / 2);

		switch (axis)
		{
			case XAxis:
			{
				for (int y = minY; y <= maxY; y++)
				{
					for (int z = minZ; z <= maxZ; z++)
					{
						voxels[GetVoxelIndex(TargetPoint.X, y, z)] = Data;
					}
				}
			}
			break;

			case YAxis:
			{
				for (int x = minX; x <= maxX; x++)
				{
					for (int z = minZ; z <= maxZ; z++)
					{
						voxels[GetVoxelIndex(x, TargetPoint.Y, z)] = Data;
					}
				}
			}
			break;

			case ZAxis:
			{
				for (int x = minX; x <= maxX; x++)
				{
					for (int y = minY; y <= maxY; y++)
					{
						voxels[GetVoxelIndex(x, y, TargetPoint.Z)] = Data;
					}
				}
			}
			break;
		}
	}
}

=======
>>>>>>> ed758910b21fbef2c4ca7be13381cb83261cf27e

