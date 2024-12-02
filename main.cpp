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

	for (int i = 1; i < argc; i++) {
		printf("[Path:");
		printf(argv[i]);
		printf("]\n");
	}
	printf("\n");

	// ファイルパスが指定されていない時
	if (argc < _NumArgment) {
		TextureConverter::OutputUsage();
		TextureConverter::OutputEnd();
		system("pause");
		return 0;
	}
	//assert(_NumArgment <= argc);


	// COM ライブラリの初期化
	HRESULT hr;
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	// テクスチャコンバータ
	TextureConverter converter;

	// テクスチャ変換
	converter.ConvertTextureWICToDDS(argv[kFilePath]);

	// COM ライブラリの終了
	CoUninitialize();

	system("pause");
	return 0;
}