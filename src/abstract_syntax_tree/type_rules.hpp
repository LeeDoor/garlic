#pragma once
#include "cell_type.hpp"

namespace garlic {

class TypeRules {
public:
    TypeRules() = delete;

    enum OperationError {
	BinaryMath, UnaryMath, Comparison
    };
    using TypeOrError = std::variant<CellType, OperationError>;

    template<CellType cell>
    static constexpr size_t get_type_size() {
	if constexpr (cell == CellType::Int) {
	    return sizeof(IntType);
	} else if constexpr (cell == CellType::Float) {
	    return sizeof(FloatType);
	} else {
	    throw std::logic_error("Unknown type to get size of");
	}
    }
    template<IsStoringColumnType T>
    static constexpr CellType get_cell_from_type() {
	if constexpr (std::is_same_v<T, IntType>) { return Int; }
	if constexpr (std::is_same_v<T, FloatType>) { return Float; }
	if constexpr (std::is_same_v<T, StringType>) { return String; }
    }

    static constexpr size_t get_type_size(CellType cell) {
	switch(cell) {
	    case CellType::Int: return get_type_size<CellType::Int>();
	    case CellType::Float: return get_type_size<CellType::Float>();
	    default: throw std::logic_error("Unknown type to get size of");
	}
    }
    static void as_str(std::ostream& os, CellType ct) {
	static std::unordered_map<CellType, StringType> map {
	    { String, "String" },
	    { Int, "Int" },
	    { Float, "Float" }
	};
	os << map[ct];
    }

    static StringType write_error(OperationError err, 
	    CellType lhs, std::optional<CellType> rhs = std::nullopt) {
	static std::unordered_map<OperationError, StringType> map {
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

    static TypeOrError binary_math_comp(CellType lhs, CellType rhs) {
	static std::map<std::pair<CellType, CellType>, CellType> map {
	    { { Int, Int }, Int },
	    { { Int, Float }, Float },
	    { { Float, Int }, Float },
	    { { Float, Float }, Float }
	};
	if(map.contains({ lhs, rhs }))
	    return map.at({ lhs, rhs });
	return BinaryMath;
    }
    static TypeOrError unary_math_comp(CellType op) {
	std::vector<CellType> acceptable = { Int, Float };
	if(std::find(acceptable.begin(), acceptable.end(), op) != acceptable.end())
	    return op;
	return UnaryMath;
    }
    static TypeOrError comparison_comp(CellType lhs, CellType rhs) {
	static std::set<std::pair<CellType, CellType>> acceptable {
	    { Int, Int },
	    { Int, Float },
	    { Float, Int },
	    { Float, Float },
	    { String, String }
	};
	if(acceptable.contains({ lhs, rhs }))
	    return Boolean;
	return Comparison;
    }
};

};
