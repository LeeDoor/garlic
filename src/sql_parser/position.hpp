#pragma once

namespace yy { class location; };

namespace garlic::sql_parser {

static constexpr size_t DEFAULT_LINE = 1;
static constexpr size_t DEFAULT_COLUMN = 1;
static constexpr size_t DEFAULT_CHARACTERS = 0;

/// Defines position in parsed text.
/// Includes both: amount of characters read and row-column location.
class Position {
public:
    explicit Position(
	    size_t line = DEFAULT_LINE, 
	    size_t col = DEFAULT_COLUMN,
	    size_t chars = DEFAULT_CHARACTERS)
    { initialize(line, col, chars); }

    operator yy::location() const;

    inline size_t get_line() const { return line_; }
    inline size_t get_column() const { return column_; }
    inline size_t get_characters() const { return characters_; }

    inline void lines(int n = 1) { 
	if(n < 0 && static_cast<size_t>(std::abs(n)) > line_) 
	    throw std::logic_error("reducing lines(n) below zero");
	line_ += n; 
	column_ = DEFAULT_COLUMN;
	characters_ += n;
    }
    inline void columns(int n = 1) {
	if(n < 0 && static_cast<size_t>(std::abs(n)) > column_) 
	    throw std::logic_error("reducing columns(n) below zero");
	column_ += n;
	characters_ += n;
    }
    inline void initialize(
	    size_t line = DEFAULT_LINE, 
	    size_t col = DEFAULT_COLUMN,
	    size_t chars = DEFAULT_CHARACTERS)
	{ line_ = line; column_ = col; characters_ = chars; }

    inline Position operator-(const Position& other) const {
	Position result{ 1, get_column(), get_characters() - other.get_characters() };
	if(get_line() > other.get_line()) 
	    result.line_ = get_line() - other.get_line() + 1;
	else if(get_line() == other.get_line()) 
	    result.column_ = get_column() - other.get_column() + 1;
	else throw std::logic_error("substracting earlier Position from later one.");
	return result;
    }
    Position& operator-=(const Position& other) {
	*this = *this - other;
	return *this;
    }
    inline Position operator+(const Position& other) const {
	return Position{ 
	    other.get_line() + get_line() - 1, 
	    other.get_column() + get_column() - 1,
	    other.get_characters() + get_characters() 
	};
    }
    Position& operator+=(const Position& other) {
	*this = *this + other;
	return *this;
    }
    auto operator<=>(const Position& other) const {
	if(get_line() < other.get_line())
	    return -1;
	if(get_line() > other.get_line())
	    return 1;
	if(get_column() < other.get_column())
	    return -1;
	if(get_column() > other.get_column())
	    return 1;
	if(get_column() == other.get_column())
	    return 0;
	throw std::logic_error("Position::operator<=> failed execution");
    }

private:
    size_t line_;
    size_t column_;
    size_t characters_;
};

inline std::ostream& operator<<(std::ostream& os, const Position& position) {
    os << position.get_line() << "." << position.get_column();
    return os;
}

}
