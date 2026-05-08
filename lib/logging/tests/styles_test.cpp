#include <doctest/doctest.h>
#include <logging/styles.h>

#include <sstream>


using namespace logging::styles;

TEST_SUITE_BEGIN("styles");

TEST_CASE("style set_color_") {
    style style(FG_BLUE);
    style.set_color_(FG_BRIGHT_WHITE);
    style.set_color_(BG_RESET);

    CHECK(style.fg == FG_BRIGHT_WHITE);
    CHECK(style.bg == BG_RESET);
}

TEST_CASE("style apply") {
    style all = BOLD | FG_BLACK | FAINT | ITALIC | FG_GREEN | BG_MAGENTA
                | UNDERLINE | LINE_THROUGH | DOUBLE_UNDERLINE | OVERLINE;
    std::string_view allseq
        = "\x1b[32m\x1b[45m\x1b[1m\x1b[2m\x1b[3m\x1b[4m\x1b[9m\x1b[21m\x1b[53m";

    SUBCASE("apply_seq") { CHECK(apply_seq(all) == allseq); }

    SUBCASE("apply") {
        std::ostringstream str;
        apply(all, str);
        CHECK(str.str() == allseq);
    }
}

TEST_CASE("style reset") {
    SUBCASE("reset_seq") { CHECK(reset_seq() == "\x1b[0m"); }

    SUBCASE("apply") {
        std::ostringstream str;
        reset(str);
        CHECK(str.str() == "\x1b[0m");
    }
}

TEST_SUITE_END();
