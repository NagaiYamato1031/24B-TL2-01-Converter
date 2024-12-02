#include "TextureConverter.h"

using namespace DirectX;
using namespace Utility;

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath) {
	// テクスチャファイル読み込み
	LoadWICTextureFromFile(filePath);
	// DDS ファイル書き出し
	SaveDDSTextureToFile();
}

void TextureConverter::OutputUsage() {
	printf("画像ファイルを WIC 形式から DDS 形式に変換します\n\n");
	printf("TextureConverter [ドライブ:][パス][ファイル名]\n\n");
	printf("-[ドライブ:][パス][ファイル名]: 変換したい WIC 形式の画像ファイルを指定\n");
}

void TextureConverter::OutputEnd() {
	printf("\nTextureConverter 終了\n");
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {
	// フォルダパスとファイル名を分離する
	fileInfo = FileInformation(ConvertMultiByteStringToWideString(filePath));

	printf(".読み込み中");

	std::wstring a = fileInfo.File();
	// WIC テクスチャ読み込み
	HRESULT hr;
	hr = LoadFromWICFile(a.c_str(), WIC_FLAGS_NONE, &metadata_, scratchImage_);
	if (FAILED(hr)) {
		printf("\r");
		printf("!読み込み失敗 ");
		wprintf(L"[%ws]\n", fileInfo.Full().c_str());
		OutputEnd();
		system("pause");
		exit(0);
	}
	printf("\r");
	printf("+読み込み完了 ");
	wprintf(L"[%ws]\n", fileInfo.Full().c_str());
}

void TextureConverter::SaveDDSTextureToFile() {
	HRESULT hr;

	printf(".ミップマップ生成中");
	// ミップマップ生成
	ScratchImage mipChain;
	hr = GenerateMipMaps(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(), scratchImage_.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(hr)) {
		printf("\r");
		printf("+ミップマップ生成完了\n");
		// イメージとメタデータを、ミップマップ版で置き換える
		scratchImage_ = std::move(mipChain);
		metadata_ = scratchImage_.GetMetadata();
	}

	printf(".圧縮中");
	// 圧縮形式に変換
	ScratchImage converted;
	hr = Compress(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_,
		DXGI_FORMAT_BC7_UNORM_SRGB, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT | TEX_COMPRESS_PARALLEL,
		1.0f, converted);
	if (SUCCEEDED(hr)) {
		printf("\r");
		printf("+圧縮完了\n");
		scratchImage_ = std::move(converted);
		metadata_ = scratchImage_.GetMetadata();
	}

	// 読み込んだテクスチャを SRGB として扱う
	metadata_.format = MakeSRGB(metadata_.format);

	// 出力ファイル名を設定する
	std::wstring filePath = fileInfo.FilePath() + L".dds";

	printf(".出力中");
	// DDS ファイル書き出し
	hr = SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));

	printf("\r");
	printf("+出力完了 ");
	wprintf(L"[%ws]\n", filePath.c_str());

	OutputEnd();
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString) {
	// ワイド文字列に変換した際の文字数を計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	// ワイド文字列
	std::wstring wString;
	wString.resize(filePathBufferSize);

	// ワイド文字列に変換
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}
