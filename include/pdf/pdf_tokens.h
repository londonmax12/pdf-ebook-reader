#pragma once

#include <string>
#include <memory>
#include <variant>

enum TokenType {
	PDF_UNKNOWN,
	PDF_STRING,
	PDF_CHAR,
	PDF_INT,
	PDF_SIZE,
	PDF_NAME,
	PDF_ENDL,
	PDF_VERSION,
	PDF_XREF,
	PDF_TRAILER,
	PDF_VAR,
	PDF_BEGIN,
	PDF_END,
	PDF_XREF_ENTRY,
	PDF_OBJ_START,
	PDF_OBJ_END,
	PDF_OBJ,
};

enum XRefStatus {
	XREF_UNKNOWN,
	XREF_STATUS_IN_USE,
	XREF_STATUS_FREE,
	XREF_REFERENCE
};
struct XRefEntry
{
	XRefEntry() = default;
	XRefEntry(size_t obj, size_t gen, XRefStatus status) : Object(obj), Generation(gen), Status(status) {}

	size_t Object;
	size_t Generation;
	XRefStatus Status;
};

struct Var {
	std::string Name;
	std::variant<size_t, XRefEntry, std::string> Value;

	Var(const std::string& name, size_t size) : Name(name), Value(size) {}
	Var(const std::string& name, XRefEntry xref) : Name(name), Value(xref) {}
	Var(const std::string& name, const std::string& string) : Name(name), Value(string) {}
};

struct Token {
	using TokenValue = std::variant<size_t, std::string, int, XRefEntry, Var>;

    Token(TokenType type, TokenValue value = {}) : Type(type), Value(value) {}

    TokenType Type = PDF_UNKNOWN;
    TokenValue Value;

    bool operator==(const TokenType& other) const { return Type == other; }
    bool operator!=(const TokenType& other) const { return Type != other; }
};