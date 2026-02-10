#pragma once
#include "pch.h"

struct CTextureInfo {
	ID3D11ShaderResourceView* pTexture{ nullptr };
	int Width{ 0 };
	int Height{ 0 };
};

std::expected<CTextureInfo, std::string> LoadTextureFromFile(const char* file_name);