#include <doctest/doctest.h>

#include "tests.h"

#include <filesystem>
#include <fstream>
#include <sstream>

TEST_SUITE_BEGIN("logging");

TEST_CASE("logging level") {
    INITIALIZE();

    SUBCASE("info") {
        logging::set_level(logging::level::info);
        logging::debug("msg");
        logging::fatal("fatal");
        logging::debug("vsesvfdsv");
        logging::info("info");

        CHECK(str.str() == time + " FATAL fatal\n" + time + " INFO info\n");
    }

    SUBCASE("off") {
        logging::set_level(logging::level::off);
        logging::debug("msg");
        logging::fatal("fatal");
        logging::debug("vsesvfdsv");
        logging::info("info");

        CHECK(str.str().empty());
    }
}

TEST_CASE("disable stdout") {
    INITIALIZE();
    logging::to_stdout(false);
    logging::debug("msg");
    logging::fatal("fatal");
    logging::debug("vsesvfdsv");
    logging::info("info");

    CHECK(str.str().empty());
}

TEST_CASE("logging to file") {
    INITIALIZE();
    logging::to_file();

    SUBCASE("") {
        logging::debug("msg");
        logging::fatal("fatal");
        logging::debug("");
        logging::info("info");

        logging::detail::get_log_file().close();
        std::ifstream logf(logging::detail::log_specs.path);
        std::ostringstream logs;
        logs << logf.rdbuf();

        CHECK(std::filesystem::exists(logging::detail::log_specs.path));
        CHECK(logs.str()
              == time + " DEBUG msg\n" + time + " FATAL fatal\n" + time
                     + " DEBUG \n" + time + " INFO info\n");

        std::filesystem::remove_all(
            logging::detail::log_specs.path.relative_path()
                .begin()
                ->string()); // cleanup
    }

    SUBCASE("custom file") {
        logging::set_file("test/my/logs/foo");
        logging::info("");
        CHECK(std::filesystem::exists(logging::detail::log_specs.path));

        std::filesystem::remove_all(
            logging::detail::log_specs.path.relative_path()
                .begin()
                ->string()); // cleanup
    }
}

TEST_CASE("logging basic") {
    INITIALIZE();

    SUBCASE("log (debug)") {
        logging::log(logging::level::debug, "hello");
        CHECK(str.str() == time + " DEBUG hello\n");
    }

    SUBCASE("debug") {
        logging::debug(21.37);
        CHECK(str.str() == time + " DEBUG 21.37\n");
    }

    SUBCASE("info") {
        logging::info("hello");
        CHECK(str.str() == time + " INFO hello\n");
    }

    SUBCASE("warn") {
        logging::warn("hello");
        CHECK(str.str() == time + " WARNING hello\n");
    }

    SUBCASE("error") {
        logging::error(2137);
        CHECK(str.str() == time + " ERROR 2137\n");
    }

    SUBCASE("fatal") {
        logging::fatal("hello");
        CHECK(str.str() == time + " FATAL hello\n");
    }
}

TEST_CASE("logging format") {
    INITIALIZE();

    SUBCASE("log (debug)") {
        logging::log(logging::level::debug, "hello {}", 2137);
        CHECK(str.str() == time + " DEBUG hello 2137\n");
    }

    SUBCASE("debug") {
        logging::debug("{}", 21.37);
        CHECK(str.str() == time + " DEBUG 21.37\n");
    }

    SUBCASE("info") {
        logging::info("{} {} {}", "hello", 420, 21);
        CHECK(str.str() == time + " INFO hello 420 21\n");
    }

    SUBCASE("warn") {
        logging::warn("{}", "test");
        CHECK(str.str() == time + " WARNING test\n");
    }

    SUBCASE("error") {
        logging::error("{}", 2137);
        CHECK(str.str() == time + " ERROR 2137\n");
    }

    SUBCASE("fatal") {
        logging::fatal("{}", 73283);
        CHECK(str.str() == time + " FATAL 73283\n");
    }
}


TEST_SUITE_END();
