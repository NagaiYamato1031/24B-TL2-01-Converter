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

	assert(_NumArgment <= argc);

	// COM ライブラリの初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	// テクスチャコンバータ
	TextrueConverter converter;

	// テクスチャ変換
	converter.ConvertTextureWICToDDS(argv[kFilePath]);

	// COM ライブラリの終了
	CoUninitialize();

	system("pause");
	return 0;
}