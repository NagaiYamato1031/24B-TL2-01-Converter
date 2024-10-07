#pragma once

#include <string>


class Utility {
public:
	/// <summary>
	/// string -> wstring 変換
	/// </summary>
	/// <param name="str">string</param>
	/// <returns>wstring</returns>
	static std::wstring ConvertString(const std::string& str);
	/// <summary>
	/// wstring -> string 変換
	/// </summary>
	/// <param name="str">wstring</param>
	/// <returns>string</returns>
	static std::string ConvertString(const std::wstring& str);
};

/// <summary>
/// ファイル自体の情報
/// </summary>
class FileInformation {
public: //** コンストラクタ **//
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FileInformation() = default;
	FileInformation(const std::string& filePath);
	FileInformation(const std::wstring& filePath);

public: //** パブリック関数 **//
	/// <summary>
	/// フルパスを取得する
	/// </summary>
	std::wstring Full() const { return directoryPath_ + fileName_ + L"." + fileExt_; }

	/// <summary>
	/// ファイルパスを取得する
	/// </summary>
	std::wstring FilePath() const { return directoryPath_ + fileName_; }

	/// <summary>
	/// ファイル名を取得する
	/// </summary>
	std::wstring File() const { return fileName_ + L"." + fileExt_; }

public: //** パブリック変数 **//
	// ディレクトリパス
	std::wstring directoryPath_ = L"";
	// ファイル名
	std::wstring fileName_ = L"";
	// ファイル拡張子
	std::wstring fileExt_ = L"";
};