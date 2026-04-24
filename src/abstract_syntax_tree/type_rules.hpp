#pragma once
#include "cell_type.hpp"

namespace garlic {

/// Static class to determine the rules of operations.
class TypeRules {
public:
    TypeRules() = delete;

    enum OperationError {
	BinaryMath, UnaryMath, Comparison
    };
    using TypeOrError = std::expected<CellType, OperationError>;

    /// Constexpr function to define type size at compile time.
    template<CellType cell>
    static constexpr size_t get_type_size() {
	if constexpr (cell == CellType::Int) {
	    return sizeof(IntType);
	} else if constexpr (cell == CellType::Float) {
	    return sizeof(FloatType);
	} else if constexpr (cell == CellType::Boolean) {
	    return sizeof(bool);
	} else {
	    static_assert(false, "Unknown type to get size of");
	}
    }
    /// Runtime function to get type size;
    static constexpr size_t get_type_size(CellType cell) {
	switch(cell) {
	    case CellType::Int: return get_type_size<CellType::Int>();
	    case CellType::Float: return get_type_size<CellType::Float>();
	    case CellType::Boolean: return get_type_size<CellType::Boolean>();
	    default: std::unreachable();
	}
    }
    /// Constexpr function to get @ref CellType enum from template parameter.
    template<IsStoringColumnType T>
    static constexpr CellType get_cell_from_type() {
	if constexpr (std::is_same_v<T, IntType>) { return Int; }
	if constexpr (std::is_same_v<T, FloatType>) { return Float; }
	if constexpr (std::is_same_v<T, StringType>) { return String; }
    }

    /// Converts @ref CellType to string using given ostream.
    static void as_str(std::ostream& os, CellType ct) {
	static std::unordered_map<CellType, StringType> map {
	    { String, "String" },
	    { Int, "Int" },
	    { Float, "Float" },
	    { Boolean, "Boolean" }
	};
	if(!map.contains(ct)) 
	    throw std::logic_error("Unimplemented type in TypeRules::as_str");
	os << map[ct];
    }

    /// Formats error to string.
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

    /// Defines the resulting type on binary math operation using given types.
    /*! @returns resulting type or error */
    static TypeOrError binary_math_comp(CellType lhs, CellType rhs) {
	static std::map<std::pair<CellType, CellType>, CellType> map {
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
    static TypeOrError unary_math_comp(CellType op) {
	std::vector<CellType> acceptable = { Int, Float };
	if(std::find(acceptable.begin(), acceptable.end(), op) != acceptable.end())
	    return op;
	return std::unexpected(UnaryMath);
    }
    /// Defines the resulting type on comparison operation using given types.
    /*! @returns resulting type or error */
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
	return std::unexpected(Comparison);
    }
};

};
