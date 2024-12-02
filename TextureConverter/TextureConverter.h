#pragma once

#include <string>
#include <Externals/DirectXTex/DirectXTex.h>

#include <Utility/Utility.h>

class TextureConverter {
public: //** パブリック関数 **//
	/// <summary>
	/// テクスチャを WIC から DDS に変換する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void ConvertTextureWICToDDS(const std::string& filePath, int numOptions = 0,char* options[] = nullptr);

	/// <summary>
	/// 使用方法を出力
	/// </summary>
	static void OutputUsage();

	/// <summary>
	/// 終了したことを出力
	/// </summary>
	static void OutputEnd();

private: //** プライベート変数 **//
	// 画像の情報
	DirectX::TexMetadata metadata_;
	// 画像イメージのコンテナ
	DirectX::ScratchImage scratchImage_;
	// ファイル自体の情報
	Utility::FileInformation fileInfo;
private: //** プライベート関数 **//
	/// <summary>
	/// テクスチャファイル読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadWICTextureFromFile(const std::string& filePath);
	/// <summary>
	/// DDS テクスチャとしてファイル書き出し
	/// </summary>
	void SaveDDSTextureToFile(int numOptions = 0, char* options[] = nullptr);
	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="mString">マルチバイト文字列</param>
	/// <returns>ワイド文字列</returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

	/// <summary>
	/// ミップマップを生成
	/// </summary>
	/// <param name="mipLevel">ミップマップレベル</param>
	void GenerateMipmap(size_t mipLevel);

};