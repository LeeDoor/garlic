#pragma once
#include "table_value_gatherer.hpp"
#include "validateable.hpp"

namespace garlic {

/// Base class for all Conditions in query.
/*! Condition is a logical entity that can be resolved as True or False. */
class Condition : public Validateable {
public:
    virtual ~Condition() = default;

    Condition(TypeRules::TypeOrError toe) : Validateable{ toe } {}

    /// Performs logical operations on underlying objects.
    /*! @throws std::logic_error may throw if did not validate first. */
    virtual bool resolve(sptr<TableValueGatherer> gatherer) const = 0;
};

}
