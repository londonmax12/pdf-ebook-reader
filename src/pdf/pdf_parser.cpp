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
        case PDF_OBJ_START:
        {
            int index = mIndex;

            GoToNextType(PDF_INT, -1);
            CResult genRes = ConvertStr<int>(GetCurrTokenValue());
            if (!genRes.Success)
                return false;
            int gen = genRes.Value;

            GoToNextType(PDF_INT, -1);
            CResult objRes = ConvertStr<size_t>(GetCurrTokenValue());
            if (!objRes.Success)
                return false;
            size_t obj = objRes.Value;

            mIndex = index;

            GoToNextType(PDF_NAME);
            Resource* resource = nullptr;
            
            std::vector<Token> objTokens;
            for (;;)
            {
                token = mTokens[mIndex];
                if (token.Type == PDF_OBJ_END)
                    break;

                objTokens.push_back(token);

                if (token.Type != PDF_NAME) {
                    mIndex++;
                    if (mIndex >= mTokens.size())
                        return false;
                    continue;
                }

                if (GetCurrTokenValue() == "Length") {
                    if (!GoToNextType(PDF_INT, 1, PDF_OBJ_END))
                        return false;

                    CResult lengthObj = ConvertStr<size_t>(GetCurrTokenValue());
                    if (!lengthObj.Success)
                        return false;

                    if (resource == nullptr || resource->Type == TYPE_NONE)
                        resource = mResources->AllocateResource<Stream>(obj, gen, Stream{ lengthObj.Value });

                    else if (resource->Type == TYPE_STREAM) {
                        if (resource->StreamValue.Size)
                            return false;

                        resource->StreamValue.Allocate(lengthObj.Value );
                    }
                }
                if (GetCurrTokenValue() == "Filter") {
                    StreamFilter filter = FILTER_UNKNOWN;
                    if (!GoToNextType(PDF_NAME, 1, PDF_OBJ_END))
                        return false;
                    token = mTokens[mIndex];

                    if (GetCurrTokenValue() == "FlateDecode")
                        filter = FILTER_FLATEDECODE;
                    
                    if (filter == FILTER_UNKNOWN)
                        return false;

                    if (resource == nullptr || resource->Type == TYPE_NONE)
                    {
                        resource = mResources->AllocateResource<Stream>(obj, gen, Stream{});
                        resource->StreamValue.Filter = filter;
                    }
                    else if (resource->Type == TYPE_STREAM) {
                        resource->StreamValue.Filter = filter;
                    }
                }
                mIndex++;
                if (mIndex >= mTokens.size())
                    return false;
            }

            if (resource)
                newTokens.push_back({ PDF_OBJ, resource->NameValue });
            else
                return false;

            break;
        }
	    case PDF_XREF:
        {
            newTokens.push_back({ PDF_XREF, "" });

            if (!GoToNextType(PDF_INT))
                return false;

            CResult r1 = ConvertStr<size_t>(GetCurrTokenValue());
            if (!r1.Success)
                return false;

            newTokens.push_back({ PDF_SIZE, r1.Value });

            if (!GoToNextType(PDF_INT))
                return false;

            CResult r2 = ConvertStr<size_t>(GetCurrTokenValue());
            if (!r2.Success)
                return false;

            newTokens.push_back({ PDF_SIZE, r2.Value });

            for (size_t j = 0; j < r2.Value; j++) {
                if (!GoToNextType(PDF_INT))
                    return false;

                CResult obj = ConvertStr<size_t>(GetCurrTokenValue());
                if (!obj.Success)
                    return false;

                if (!GoToNextType(PDF_INT))
                    return false;

                CResult gen = ConvertStr<size_t>(GetCurrTokenValue());
                if (!gen.Success)
                    return false;

                if (!GoToNextType(PDF_CHAR))
                    return false;

                auto status = (GetCurrTokenValue() == "f" ? XREF_STATUS_FREE : XREF_STATUS_IN_USE);
                newTokens.push_back({ PDF_XREF_ENTRY, XRefEntry(obj.Value, gen.Value, status)});
            }

            break;
        }
        case PDF_TRAILER:
        {
            newTokens.push_back({ PDF_TRAILER });
            for (int i = 0; i < 4; i++) {
                if (!GoToNextType(PDF_NAME))
                    return false;

                std::string name = GetCurrTokenValue();
                if (name == "Size") {
                    if (!GoToNextType(PDF_INT))
                        return false;

                    CResult res = ConvertStr<size_t>(GetCurrTokenValue());
                    if (!res.Success)
                        return false;

                    newTokens.push_back({ PDF_VAR, Var("Size", res.Value)});
                }
                else if (name == "Root") {
                    if (!GoToNextType(PDF_INT))
                        return false;

                    CResult obj = ConvertStr<size_t>(GetCurrTokenValue());
                    if (!obj.Success)
                        return false;

                    if (!GoToNextType(PDF_INT))
                        return false;

                    CResult gen = ConvertStr<size_t>(GetCurrTokenValue());
                    if (!gen.Success)
                        return false;

                    if (!GoToNextType(PDF_CHAR))
                        return false;

                    newTokens.push_back({ PDF_VAR, Var("Root", XRefEntry(obj.Value, gen.Value, XREF_REFERENCE))});
                }
                else if (name == "Info") {
                    if (!GoToNextType(PDF_INT))
                        return false;

                    CResult obj = ConvertStr<size_t>(GetCurrTokenValue());
                    if (!obj.Success)
                        return false;

                    if (!GoToNextType(PDF_INT))
                        return false;

                    CResult gen = ConvertStr<size_t>(GetCurrTokenValue());
                    if (!gen.Success) 
                        return false;

                    if (!GoToNextType(PDF_CHAR))
                        return false;

                    newTokens.push_back({ PDF_VAR, Var("Info", XRefEntry(obj.Value, gen.Value, XREF_REFERENCE)) });
                }
                else if (name == "ID") {
                    if (!GoToNextType(PDF_STRING))
                        return false;
                    std::string fileId = GetCurrTokenValue();
                    newTokens.push_back({ PDF_VAR, Var("FileID", fileId.substr(2, fileId.length() - 3)) });

                    if (!GoToNextType(PDF_STRING))
                        return false;
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

bool PDFParser::GoToNextType(TokenType type, int step, TokenType stopAt)
{
    mIndex += step;
    if (mIndex >= mTokens.size())
        return false;

    Token t = mTokens[mIndex];
    while (mIndex < mTokens.size()) {
        if (t.Type == type)
            return true;
        else if (t.Type == stopAt)
            return false;

        mIndex += step;
        if (mIndex >= mTokens.size())
            return false;
        t = mTokens[mIndex];
    }
    return false;
}
