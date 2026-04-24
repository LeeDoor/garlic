#include "type_rules.hpp"

namespace garlic {

void TypeRules::as_str(std::ostream& os, CellType ct) {
    std::unordered_map<CellType, StringType> map {
	{ String, "String" },
	{ Int, "Int" },
	{ Float, "Float" },
	{ Boolean, "Boolean" }
    };
    if(!map.contains(ct)) 
	throw std::logic_error("Unimplemented type in TypeRules::as_str");
    os << map[ct];
}
StringType TypeRules::write_error(OperationError err, 
	CellType lhs, std::optional<CellType> rhs) {
    std::unordered_map<OperationError, StringType> map {
	{ BinaryMath, "binary mathematical operation" },
	{ UnaryMath,  "unary mathematical operation"  },
	{ Comparison, "comparison" }
    };
    std::stringstream ss;
    ss << "Failed to execute " << map[err] << " with operands: ";
    as_str(ss, lhs); 
    if(rhs) {
	ss << " and ";
	as_str(ss, *rhs); 
    }
    ss << ".";
    return ss.str();
}
TypeRules::TypeOrError TypeRules::binary_math_comp(CellType lhs, CellType rhs) {
    std::map<std::pair<CellType, CellType>, CellType> map {
	{ { Int, Int }, Int },
	{ { Int, Float }, Float },
	{ { Float, Int }, Float },
	{ { Float, Float }, Float }
    };
    if(map.contains({ lhs, rhs }))
	return map.at({ lhs, rhs });
    return std::unexpected(BinaryMath);
}
/// Defines the resulting type on unary math operation using given types.
/*! @returns resulting type or error */
TypeRules::TypeOrError TypeRules::unary_math_comp(CellType op) {
    std::vector<CellType> acceptable = { Int, Float };
    if(std::find(acceptable.begin(), acceptable.end(), op) != acceptable.end())
	return op;
    return std::unexpected(UnaryMath);
}
/// Defines the resulting type on comparison operation using given types.
/*! @returns resulting type or error */
TypeRules::TypeOrError TypeRules::comparison_comp(CellType lhs, CellType rhs) {
    std::set<std::pair<CellType, CellType>> acceptable {
	{ Int, Int },
	{ Int, Float },
	{ Float, Int },
	{ Float, Float },
	{ String, String }
    };
    if(acceptable.contains({ lhs, rhs }))
	return Boolean;
    return std::unexpected(Comparison);
}

}
