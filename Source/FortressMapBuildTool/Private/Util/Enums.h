#pragma once

enum class EDirection
{
	Forward, Right,Back, Left, Up, Down
};

enum class EBlock
{
	Null, Air, Stone, Dirt, Grass
};

enum class EBlockType
{
    None,      // ���� ����
    Cube,      // �⺻ �簢 ť��
    PosX_NegZ_Slope, 
    PosX_PosZ_Slope, 
    NegX_NegZ_Slope, 
    NegX_PosZ_Slope, 
    PosY_NegZ_Slope, 
    PosY_PosZ_Slope, 
    NegY_NegZ_Slope, 
    NegY_PosZ_Slope


};