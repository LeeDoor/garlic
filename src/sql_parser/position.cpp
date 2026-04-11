#include "position.hpp"
#include "location.hh"

namespace garlic::sql_parser {

    Position::operator yy::location() const {
	return yy::location{ 
	    YY_NULLPTR, 
	    static_cast<int>(get_line()), 
	    static_cast<int>(get_column()) 
	};
    }

}
