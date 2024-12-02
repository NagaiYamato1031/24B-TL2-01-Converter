#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <ComAdmin.h>

#include <TextureConverter/TextureConverter.h>

// コマンドライン引数
enum Argment {
	kApplicationPath = 0,	// アプリケーションのパス
	kFilePath,				// 渡されたファイルのパス

	_NumArgment,
};

int main(int argc, char* argv[]) {

	printf("TextureConverter 起動\n");

	// ファイルパスが指定されていない時
	if (argc < _NumArgment) {
		TextureConverter::OutputUsage();
		TextureConverter::OutputEnd();
		system("pause");
		return 0;
	}

	// COM ライブラリの初期化
	HRESULT hr;
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	// テクスチャコンバータ
	TextureConverter converter;

	// オプション数
	int numOptions = argc - _NumArgment;
	// オプション配列(ダブルポインタ)
	char** options = argv + _NumArgment;

	// パスを表示
	printf("[Path:");
	printf(argv[kFilePath]);
	printf("]\n");
	// コマンドオプションを表示
	printf("Option:");
	for (int i = 2; i < argc; i++) {
		printf(argv[i]);
		printf(" ");
	}
	printf("\n");
	printf("\n");


	// テクスチャ変換
	converter.ConvertTextureWICToDDS(argv[kFilePath], numOptions, options);

	// COM ライブラリの終了
	CoUninitialize();

	system("pause");
	return 0;
}