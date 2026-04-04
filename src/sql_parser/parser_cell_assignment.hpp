#pragma once
#include "driver.hpp"
#include "parser.tab.hpp"

namespace garlic::sql_parser {

template<typename T, typename... Args>
std::unique_ptr<T> assign_or_abort(driver& drv, Args&&... args) {
    auto obj = std::make_unique<T>(std::forward<Args>(args)...);
    if constexpr (requires (const T& t) { t.validate(); }) {
	if(auto error = obj->validate()) {
	    drv.invoke_error(ErrorStage::SemanticAnalysis, *error);
	    return nullptr;
	}
    }
    return obj;
}

}
