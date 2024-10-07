#include "TextureConverter.h"

using namespace DirectX;

void TextrueConverter::ConvertTextureWICToDDS(const std::string& filePath) {
	// テクスチャファイル読み込み
	LoadWICTextureFromFile(filePath);
	// DDS ファイル書き出し
	SaveDDSTextureToFile();
}

void TextrueConverter::LoadWICTextureFromFile(const std::string& filePath) {
	// フォルダパスとファイル名を分離する
	fileInfo = FileInformation(filePath);

	std::wstring a = fileInfo.Full();
	// WIC テクスチャ読み込み
	HRESULT hr = LoadFromWICFile(a.c_str(), WIC_FLAGS_NONE, &metadata_, scratchImage_);
	assert(SUCCEEDED(hr));
}

void TextrueConverter::SaveDDSTextureToFile() {
	// 読み込んだテクスチャを SRGB として扱う
	metadata_.format = MakeSRGB(metadata_.format);

	// 出力ファイル名を設定する
	std::wstring filePath = fileInfo.FilePath() + L".dds";

	// DDS ファイル書き出し
	HRESULT hr = SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));
}
