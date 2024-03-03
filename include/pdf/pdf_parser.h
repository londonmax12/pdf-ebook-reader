#pragma once

#include "pdf/pdf_tokens.h"
#include "pdf/pdf_tokenizer.h"

class PDFParser {
public:
	PDFParser(std::vector<Token>& tokens) : mTokens(tokens) {};
	bool Parse();
private:
	std::vector<Token>& mTokens;
	int mIndex = 0;

	bool GoToNextType(TokenType type);

	template <typename T>
	struct CResult {
		bool Success;
		T Value;
	};

	template <typename T>
	CResult<T> ConvertStr(const std::string& str) {
		try {
			if constexpr (std::is_same_v<T, int>) {
				return { true, std::stoi(str) };
			}
			else if constexpr (std::is_same_v<T, size_t>) {
				return { true, std::stoul(str) };
			}
			else {
				return { false, T() };
			}
		}
		catch (const std::exception&) {
			return { false, T() };
		}
	}

	template <typename T>
	T GetCurrTokenValue() {
		Token t = mTokens[mIndex];
		return std::get<T>(t.Value);
	}

	std::string GetCurrTokenValue() {
		Token t = mTokens[mIndex];
		return std::get<std::string>(t.Value);
	};
};