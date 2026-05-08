#include "table_query_result.hpp"
#include "manual_io.hpp"

namespace garlic {
namespace {

struct CliZebraStyle {
    std::string_view default_cell_prefix = "\033[0m";
    std::string_view inverted_cell_prefix = "\033[7m";
    std::string_view cell_suffix = "\033[0m";
};

class ScopedManualIOMode {
public:
    explicit ScopedManualIOMode(bool value)
    : old_value_{ is_manual_IO() } {
        set_manual_IO(value);
    }

    ~ScopedManualIOMode() {
        set_manual_IO(old_value_);
    }

private:
    bool old_value_;
};

std::string wrap_cells(const std::vector<std::string_view>& cells, std::string_view prefix, const CliZebraStyle& style) {
    std::string row;
    row += '|';
    row += prefix;
    for (std::size_t i = 0; i < cells.size(); ++i) {
        row += cells[i];
        if (i + 1 < cells.size()) {
            row += '|';
        }
    }
    row += style.cell_suffix;
    row += '|';
    row += '\n';
    return row;
}

std::string wrap_plain_cells(const std::vector<std::string_view>& cells) {
    std::string row;
    row += '|';
    for (std::size_t i = 0; i < cells.size(); ++i) {
        row += cells[i];
        if (i + 1 < cells.size()) {
            row += '|';
        }
    }
    row += '|';
    row += '\n';
    return row;
}

}


TEST(test_table_query_result, cliZebraKeepsHeaderAndFirstBodyRowDefault) {
    ScopedManualIOMode cli_mode(true);

    ResultTable table{
        { "Int" },
        { "1" },
        { "2" },
        { "3" },
        { "4" },
    };

    TableQueryResult result(std::move(table));

    const std::string expected =
        "+-----+\n"
        + wrap_plain_cells({ " Int " })
        + "+-----+\n"
        + wrap_plain_cells({ " 1   " })
        + wrap_plain_cells({ " 2   " })
        + wrap_plain_cells({ " 3   " })
        + wrap_plain_cells({ " 4   " })
        + "+-----+\n";

    EXPECT_EQ(result.format(), expected);
}

TEST(test_table_query_result, cliZebraAlternatesWholeMulticolumnRows) {
    ScopedManualIOMode cli_mode(true);

    ResultTable table{
        { "Int", "String" },
        { "1", "a" },
        { "2", "bbb" },
        { "3", "cc" },
    };

    TableQueryResult result(std::move(table));

    const std::string expected =
        "+-----+--------+\n"
        + wrap_plain_cells({ " Int ", " String " })
        + "+-----+--------+\n"
        + wrap_plain_cells({ " 1   ", " a      " })
        + wrap_plain_cells({ " 2   ", " bbb    " })
        + wrap_plain_cells({ " 3   ", " cc     " })
        + "+-----+--------+\n";

    EXPECT_EQ(result.format(), expected);
}

TEST(test_table_query_result, cliZebraKeepsMultilineStringHighlightInsideCellBorders) {
    ScopedManualIOMode cli_mode(true);
    CliZebraStyle style;

    ResultTable table{
        { "String" },
        { "hello\nthere\nhi" },
        { "x" },
    };

    TableQueryResult result(std::move(table));

    const std::string expected =
        "+--------+\n"
        + wrap_cells({ " String " }, style.default_cell_prefix, style)
        + "+--------+\n"
        + wrap_cells({ " hello +" }, style.default_cell_prefix, style)
        + wrap_cells({ " there +" }, style.default_cell_prefix, style)
        + wrap_cells({ " hi     " }, style.default_cell_prefix, style)
        + wrap_cells({ " x      " }, style.inverted_cell_prefix, style)
        + "+--------+\n";

    EXPECT_EQ(result.format(), expected);
}

TEST(test_table_query_result, cliZebraHandlesTabsAndNewlinesInsideCells) {
    ScopedManualIOMode cli_mode(true);
    CliZebraStyle style;

    ResultTable table{
        { "String" },
        { "A\tB\nCC" },
        { "x\tY\nZ" },
    };

    TableQueryResult result(std::move(table));

    const std::string expected =
        "+--------+\n"
        + wrap_cells({ " String " }, style.default_cell_prefix, style)
        + "+--------+\n"
        + wrap_cells({ " A -> B+" }, style.default_cell_prefix, style)
        + wrap_cells({ " CC     " }, style.default_cell_prefix, style)
        + wrap_cells({ " x -> Y+" }, style.inverted_cell_prefix, style)
        + wrap_cells({ " Z      " }, style.inverted_cell_prefix, style)
        + "+--------+\n";

    EXPECT_EQ(result.format(), expected);
}

}
