#include "TextureConverter.h"

using namespace DirectX;
using namespace Utility;

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath, int numOptions, char* options[]) {
	OutputStart();
	
	// テクスチャファイル読み込み
	LoadWICTextureFromFile(filePath);
	// DDS ファイル書き出し
	SaveDDSTextureToFile(numOptions, options);

	// 実行チェック
	if (commandFlag_ & COMMANDS::cCheck) {
		printf("\n-check コマンドが実行されました。停止中です。\n\n");
		system("pause");
	}

	OutputEnd();
}


void TextureConverter::OutputUsage() {
	printf("画像ファイルを WIC 形式から DDS 形式に変換します\n\n");
	printf("TextureConverter [ドライブ:][パス][ファイル名] [-ml level] [-check]\n\n");
	printf("-[ドライブ:][パス][ファイル名]: 変換したい WIC 形式の画像ファイルを指定\n\n");
	printf("-[-ml level]: ミップレベルを指定。 0 を指定すると 1x1 までのフルミップマップチェーンを生成します。\n\n");
	printf("-[-check]: -check コマンドで実行後停止します。指定しない場合停止しません。\n\n");
}

void TextureConverter::OutputStart() {
	printf("\nTextureConverter 起動\n\n");
}

void TextureConverter::OutputEnd() {
	printf("\nTextureConverter 終了\n");
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {
	// フォルダパスとファイル名を分離する
	fileInfo_ = FileInformation(ConvertMultiByteStringToWideString(filePath));

	printf(".読み込み中");

	std::wstring a = fileInfo_.File();
	// WIC テクスチャ読み込み
	HRESULT hr;
	hr = LoadFromWICFile(a.c_str(), WIC_FLAGS_NONE, &metadata_, scratchImage_);
	if (FAILED(hr)) {
		printf("\r\033[2K");
		printf("!読み込み失敗 ");
		wprintf(L"[%ws]\n", fileInfo_.Full().c_str());
		OutputEnd();
		system("pause");
		exit(0);
	}
	printf("\r\033[2K");
	printf(">読み込み完了 ");
	wprintf(L"[%ws]\n", fileInfo_.Full().c_str());
}

void TextureConverter::SaveDDSTextureToFile(int numOptions, char* options[]) {
	HRESULT hr;

	// ミップレベル
	size_t mipLevel = 0;

	// コマンド情報を取得
	for (int i = 0; i < numOptions; i++) {
		// ミップマップを生成するか
		if (std::string(options[i]) == "-ml") {
			// ミップレベル指定
			mipLevel = std::stoi(options[i + 1]);
			// ミップマップ生成
			commandFlag_ = commandFlag_ | COMMANDS::cMip;
		}
		// 実行チェックするか
		if (std::string(options[i]) == "-check") {
			// チェックする
			commandFlag_ = commandFlag_ | COMMANDS::cCheck;
		}
	}

	// ミップマップコマンドがあるか
	if (commandFlag_ & COMMANDS::cMip) {
		// ミップマップ生成
		GenerateMipmap(mipLevel);
	}

	printf(".圧縮中");
	// 圧縮形式に変換
	ScratchImage converted;
	hr = Compress(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_,
		DXGI_FORMAT_BC7_UNORM_SRGB, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT | TEX_COMPRESS_PARALLEL,
		1.0f, converted);
	if (SUCCEEDED(hr)) {
		printf("\r\033[2K");
		printf(">圧縮完了\n");
		scratchImage_ = std::move(converted);
		metadata_ = scratchImage_.GetMetadata();
	}

	// 読み込んだテクスチャを SRGB として扱う
	metadata_.format = MakeSRGB(metadata_.format);

	// 出力ファイル名を設定する
	std::wstring filePath = fileInfo_.FilePath() + L".dds";

	printf(".出力中");
	// DDS ファイル書き出し
	hr = SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));

	printf("\r\033[2K");
	printf(">出力完了 ");
	wprintf(L"[%ws]\n", filePath.c_str());
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

void TextureConverter::GenerateMipmap(size_t mipLevel) {
	HRESULT hr = S_FALSE;

	printf("+ミップマップ レベル:%d\n", (int)mipLevel);
	printf(".ミップマップ生成中");
	// ミップマップ生成
	ScratchImage mipChain;
	hr = GenerateMipMaps(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(), scratchImage_.GetMetadata(),
		TEX_FILTER_DEFAULT, mipLevel, mipChain);
	if (SUCCEEDED(hr)) {
		printf("\r\033[2K");
		printf("+生成完了\n");
		// イメージとメタデータを、ミップマップ版で置き換える
		scratchImage_ = std::move(mipChain);
		metadata_ = scratchImage_.GetMetadata();
	}
}
