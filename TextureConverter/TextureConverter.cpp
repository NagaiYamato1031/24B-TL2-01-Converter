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
	HRESULT hr;

	// ミップマップ生成
	ScratchImage mipChain;
	hr = GenerateMipMaps(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(), scratchImage_.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(hr)) {
		// イメージとメタデータを、ミップマップ版で置き換える
		scratchImage_ = std::move(mipChain);
		metadata_ = scratchImage_.GetMetadata();
	}

	// 圧縮形式に変換
	ScratchImage converted;
	hr = Compress(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_,
		DXGI_FORMAT_BC7_UNORM_SRGB, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT | TEX_COMPRESS_PARALLEL,
		1.0f, converted);
	if (SUCCEEDED(hr)) {
		scratchImage_ = std::move(converted);
		metadata_ = scratchImage_.GetMetadata();
	}

	// 読み込んだテクスチャを SRGB として扱う
	metadata_.format = MakeSRGB(metadata_.format);

	// 出力ファイル名を設定する
	std::wstring filePath = fileInfo.FilePath() + L".dds";

	// DDS ファイル書き出し
	hr = SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));
}
