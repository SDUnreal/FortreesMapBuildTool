// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel/ChunkWorld.h"

#include "Misc/FileHelper.h"
#include "JsonObjectConverter.h"
#include "Templates/SharedPointer.h"
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
	voxels.Init(0, (ChunkSize + 1) * (ChunkSize + 1) * (ChunkSize + 1));
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
// TODO : 현재 맵을 세이브 하게 하는 기능 -> Json으로sav파일저장.
	IsSelect = false;
	while (IsSelect == false)
	{
		this->saveExplorer(directories, IsSelect);
	}
	FString JsonString;
	data.voxels = voxels;
	
	directory = directories[0];
	FJsonObjectConverter::UStructToJsonObjectString(data, JsonString);
	FFileHelper::SaveStringToFile(*JsonString, *directory);
	directory = TEXT("");
	directories.Pop();
}


void AChunkWorld::LoadMaps()
{
	// path에서 로드할 수 있게 해줌 
	IsSelect = false;
	while (IsSelect == false)
	{
		this->LoadExplorer(directories, IsSelect);
	}
	directory = directories[0];
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString,*(directory));
	TSharedRef<TJsonReader<TCHAR>>  JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		data.MapName = JsonObject->GetStringField("mapName");
		data.TestNum = JsonObject->GetIntegerField("testNum");
		TArray<TSharedPtr<FJsonValue>> JsonValueArray = JsonObject->GetArrayField("voxels");

		for (int32 i = 0; i < JsonValueArray.Num(); i++)
		{
			data.voxels[i] = JsonValueArray[i]->AsNumber();
		}
		voxels = data.voxels;
		UE_LOG(LogTemp,Warning,TEXT("SD Unreal : Deserialize success!"));
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("SD Unreal : Deserialize failure"));
	}
	directory = TEXT("");
	directories.Pop();
}

void AChunkWorld::LoadExplorer(TArray<FString>& Directories, bool& bIsSelect)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	FString DefaultFile = TEXT("");
	FString DefaultPath = TEXT("/.");
	FString DirectoryName = TEXT("");

	if (DesktopPlatform) {
		DesktopPlatform->OpenFileDialog(
			NULL,
			TEXT("Select file"),	// 파일탐색기 제목
			DefaultPath,	// 열고싶은 파일탐색기 경로
			DefaultFile,	// 동작하지 않는 파라미터같음 (지정해도 파일 안열림)
			TEXT("json Files (*.json)|*.json|"),	// 열고싶은 파일 종류
			EFileDialogFlags::None,
			Directories // 디렉토리의 파일 목록이 여기로 들어옴
		);
	}
	if (Directories.Num() ==0) bIsSelect = false;
	else bIsSelect = true;
}

void AChunkWorld::saveExplorer(TArray<FString>& Directories, bool& bIsSelect)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	FString DefaultFile = TEXT("");
	FString DefaultPath = TEXT("/.");
	FString DirectoryName = TEXT("");
	if (DesktopPlatform) {
		DesktopPlatform->SaveFileDialog(
			NULL,
			TEXT("Save file"),
			DefaultPath,
			DefaultFile,
			TEXT("json Files(*.json)|*.json|"),
			EFileDialogFlags::None,
			Directories
		);
	}
	if (Directories.Num() == 0) bIsSelect = false;
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


