#include "literals_converter.hpp"

namespace garlic::sql_parser {

template<typename T> requires std::is_arithmetic_v<T>
std::optional<T> make_number(std::string_view s) {
    T result;
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), result);
    if (ec == std::errc::result_out_of_range) {
	return std::nullopt;
    } else if (ec == std::errc::invalid_argument) {
	throw std::logic_error("Regular expression matched wrong number");
    }
    return result;
}

yy::parser::symbol_type make_FLOAT(std::string_view s, const Position& curloc, ParsingSession& session) {
    if(auto num = make_number<FloatType>(s)) {
	return yy::parser::make_FLOAT(*num, curloc);
    }
    return session.lexing_error("Failed to convert \"" + std::string(s) + "\" to float; too big value");
}
yy::parser::symbol_type make_INTEGER(std::string_view s, const Position& curloc, ParsingSession& session) {
    if(auto num = make_number<IntType>(s)) {
	return yy::parser::make_INTEGER(*num, curloc);
    }
    return session.lexing_error("Failed to convert \"" + std::string(s) + "\" to int; too big value");
}
yy::parser::symbol_type make_STRING(const std::string& s, const Position& curloc) {
    std::string result; result.reserve(s.size() - 2);
    for(size_t i = 1; i < s.size() - 1; ++i) {
	if(s[i] == '\\') {
	    switch(s[++i]) {
		case 'n':
		    result.push_back('\n');
		    break;
		case 't':
		    result.push_back('\t');
		    break;
		case '\\':
		    result.push_back('\\');
		    break;
		case '\'':
		    result.push_back('\'');
		    break;
		case '\"': // " -- character to fix broken highlight in .ll file
		    result.push_back('\"');// "
		    break;
		default:
		    result.push_back(s[--i]);
		    break;
	    }
	} else { 
	    result.push_back(s[i]);
	}
    }
    return yy::parser::make_STRING(std::move(result), curloc);
}

}
