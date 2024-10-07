#pragma once

#include <string>
#include <Externals/DirectXTex/DirectXTex.h>

#include <Utility/Utility.h>

class TextrueConverter {
public: //** パブリック関数 **//
	/// <summary>
	/// テクスチャを WIC から DDS に変換する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void ConvertTextureWICToDDS(const std::string& filePath);

private: //** プライベート変数 **//
	// 画像の情報
	DirectX::TexMetadata metadata_;
	// 画像イメージのコンテナ
	DirectX::ScratchImage scratchImage_;
	// ファイル自体の情報
	FileInformation fileInfo;
private: //** プライベート関数 **//
	/// <summary>
	/// テクスチャファイル読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadWICTextureFromFile(const std::string& filePath);
	/// <summary>
	/// DDS テクスチャとしてファイル書き出し
	/// </summary>
	void SaveDDSTextureToFile();

};