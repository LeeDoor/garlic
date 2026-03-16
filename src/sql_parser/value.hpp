#pragma once
#include <compare>
#include <ostream>

class Value {
public:
    enum Type { INT, FLOAT };

    Value(int val);
    Value(float val);
    Value();

    Value operator+(Value other) const;
    Value operator-(Value other) const;
    Value operator*(Value other) const;
    Value operator/(Value other) const;
    Value operator%(Value other) const;

    Type get_type() const;
    int to_int() const;
    float to_float() const;
    std::partial_ordering operator<=>(const Value& other) const;
    bool operator==(const Value& other) const;
    Value abs() const;

private:
    union {
        int intval_;
        float floatval_;
    };
    Type type_;
};

std::ostream& operator<<(std::ostream& os, const Value& value);
