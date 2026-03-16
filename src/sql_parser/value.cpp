#include "value.hpp"
#include <cmath>

Value::Value(int val)
    : intval_{val}
    , type_{INT}
{}
Value::Value(float val)
    : floatval_{val}
    , type_{FLOAT}
{}
Value::Value()
    : Value(0)
{}
#define VALUE_OPERATOR(operation) \
Value Value::operator operation(Value other) const { \
    if(get_type() == INT && other.get_type() == INT) { \
        return Value{ to_int() operation other.to_int() }; \
    }\
    return Value{ to_float() operation other.to_float() };\
}
VALUE_OPERATOR(+)
VALUE_OPERATOR(-)
VALUE_OPERATOR(*)
VALUE_OPERATOR(/)

Value Value::operator%(Value other) const {
    if(get_type() == INT && other.get_type() == INT) {
        return Value{ to_int() % other.to_int() }; 
    }
    return Value{ std::fmod(to_float(), other.to_float()) };
}

Value::Type Value::get_type() const { return type_; }
int Value::to_int() const { 
    return type_ == FLOAT ? static_cast<int>(floatval_) : intval_;
}
float Value::to_float() const {
    return type_ == INT ? static_cast<float>(intval_) : floatval_;
}

std::partial_ordering Value::operator<=>(const Value& other) const {
    if(get_type() == INT && other.get_type() == INT)
        return to_int() <=> other.to_int();
    return to_float() <=> other.to_float();
}
bool Value::operator==(const Value& other) const {
    if(get_type() == INT && other.get_type() == INT)
        return to_int() == other.to_int();
    return to_float() == other.to_float();
}
Value Value::abs() const {
    Value ret = *this;
    if(ret.get_type() == INT) ret.intval_ = std::abs(ret.intval_);
    else ret.floatval_ = std::abs(ret.floatval_);
    return ret;
}
std::ostream& operator<<(std::ostream& os, const Value& value) {
    switch(value.get_type()) {
        case Value::INT:
            os << value.to_int();
            break;
        case Value::FLOAT:
            os << value.to_float();
            break;
    }
    return os;
}
