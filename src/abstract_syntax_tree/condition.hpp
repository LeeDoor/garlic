#pragma once
#include "table_value_gatherer.hpp"
#include "validateable.hpp"

namespace garlic {

/// Base class for all Conditions in query.
/*! Condition is a logical entity that can be resolved as True or False. */
class Condition : public CanBeValidated {
public:
    virtual ~Condition() = default;

    Condition(TypeRules::TypeOrError toe) : CanBeValidated{ toe } {}

    using ErrorType = StringType;
    using ExpectedBoolean = std::expected<bool, ErrorType>;

    /// Performs logical operations on underlying objects.
    /*! @throws std::logic_error may throw if did not validate first. */
    virtual ExpectedBoolean resolve(sptr<TableValueGatherer> gatherer) const = 0;
};

}
