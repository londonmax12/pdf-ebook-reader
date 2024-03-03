#include "pdf/pdf_parser.h"

bool PDFParser::Parse()
{
	std::vector<Token> newTokens{};

	for (mIndex = 0; mIndex < mTokens.size(); mIndex++) {
        Token token = mTokens[mIndex];
		switch (token.Type)
		{
        case PDF_VERSION:
        {
            newTokens.push_back(token);
            break;
        }
	    case PDF_XREF:
        {
            newTokens.push_back({ PDF_XREF, "" });

            if (!GoToNextType(PDF_INT)) return false;

            CResult r1 = ConvertStr<size_t>(GetCurrTokenValue());
            if (!r1.Success) return false;

            newTokens.push_back({ PDF_SIZE, r1.Value });

            if (!GoToNextType(PDF_INT)) return false;

            CResult r2 = ConvertStr<size_t>(GetCurrTokenValue());
            if (!r2.Success) return false;

            newTokens.push_back({ PDF_SIZE, r2.Value });

            for (size_t j = 0; j < r2.Value; j++) {
                if (!GoToNextType(PDF_INT)) return false;

                CResult obj = ConvertStr<size_t>(GetCurrTokenValue());
                if (!obj.Success) return false;

                if (!GoToNextType(PDF_INT)) return false;

                CResult gen = ConvertStr<size_t>(GetCurrTokenValue());
                if (!gen.Success) return false;

                if (!GoToNextType(PDF_CHAR)) return false;

                auto status = (GetCurrTokenValue() == "f" ? XREF_STATUS_FREE : XREF_STATUS_IN_USE);
                newTokens.push_back({ PDF_XREF_ENTRY, XRefEntry(obj.Value, gen.Value, status)});
            }

            break;
        }
        case PDF_TRAILER:
        {
            newTokens.push_back({ PDF_TRAILER });
            for (int i = 0; i < 4; i++) {
                if (!GoToNextType(PDF_NAME)) return false;

                std::string name = GetCurrTokenValue();
                if (name == "Size") {
                    if (!GoToNextType(PDF_INT)) return false;

                    CResult res = ConvertStr<size_t>(GetCurrTokenValue());
                    if (!res.Success) return false;

                    newTokens.push_back({ PDF_VAR, Var("Size", res.Value)});
                }
                else if (name == "Root") {
                    if (!GoToNextType(PDF_INT)) return false;

                    CResult obj = ConvertStr<size_t>(GetCurrTokenValue());
                    if (!obj.Success) return false;

                    if (!GoToNextType(PDF_INT)) return false;

                    CResult gen = ConvertStr<size_t>(GetCurrTokenValue());
                    if (!gen.Success) return false;

                    if (!GoToNextType(PDF_CHAR)) return false;

                    newTokens.push_back({ PDF_VAR, Var("Root", XRefEntry(obj.Value, gen.Value, XREF_REFERENCE))});
                }
                else if (name == "Info") {
                    if (!GoToNextType(PDF_INT)) return false;

                    CResult obj = ConvertStr<size_t>(GetCurrTokenValue());
                    if (!obj.Success) return false;

                    if (!GoToNextType(PDF_INT)) return false;

                    CResult gen = ConvertStr<size_t>(GetCurrTokenValue());
                    if (!gen.Success) return false;

                    if (!GoToNextType(PDF_CHAR)) return false;

                    newTokens.push_back({ PDF_VAR, Var("Info", XRefEntry(obj.Value, gen.Value, XREF_REFERENCE)) });
                }
                else if (name == "ID") {
                    if (!GoToNextType(PDF_STRING)) return false;
                    std::string fileId = GetCurrTokenValue();
                    newTokens.push_back({ PDF_VAR, Var("FileID", fileId.substr(2, fileId.length() - 3)) });

                    if (!GoToNextType(PDF_STRING)) return false;
                    std::string updateId = GetCurrTokenValue();
                    newTokens.push_back({ PDF_VAR, Var("UpdateID", updateId.substr(1, updateId.length() - 3)) });
                }
            }
            break;
        }
		default:
			break;
		}
	}
    mTokens.clear();
    mTokens = newTokens;
    return true;
}

bool PDFParser::GoToNextType(TokenType type)
{
    mIndex++;
    if (mIndex >= mTokens.size())
        return false;

    Token t = mTokens[mIndex];
    while (mIndex < mTokens.size()) {
        if (t.Type == type)
            return true;
        mIndex++;
        t = mTokens[mIndex];
    }
    return false;
}
