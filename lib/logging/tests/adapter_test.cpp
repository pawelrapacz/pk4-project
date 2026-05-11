#include <doctest/doctest.h>

#include "tests.h"

TEST_SUITE_BEGIN("adapters");

#ifdef LOGGING_LLAMA_ADAPTER

    #include <llama.h>
    #include <logging/adapters/llama.h>

TEST_CASE("llama log") {
    INITIALIZE();
    ggml_log_callback logger = logging::llama_callback;

    SUBCASE("debug") {
        logger(GGML_LOG_LEVEL_DEBUG, "21.37", nullptr);
        CHECK(str.str() == time + " DEBUG 21.37\n");
    }

    SUBCASE("info") {
        logger(GGML_LOG_LEVEL_INFO, "hello", nullptr);
        CHECK(str.str() == time + " INFO hello\n");
    }

    SUBCASE("warn") {
        logger(GGML_LOG_LEVEL_WARN, "hello", nullptr);
        CHECK(str.str() == time + " WARNING hello\n");
    }

    SUBCASE("error") {
        logger(GGML_LOG_LEVEL_ERROR, "2137", nullptr);
        CHECK(str.str() == time + " ERROR 2137\n");
    }

    SUBCASE("none") {
        logger(GGML_LOG_LEVEL_NONE, "hello", nullptr);
        CHECK(str.str() == time + " INFO hello\n");
    }

    SUBCASE("cont") {
        logger(GGML_LOG_LEVEL_INFO, "hello", nullptr);
        logger(GGML_LOG_LEVEL_ERROR, "hello", nullptr);
        logger(GGML_LOG_LEVEL_CONT, "hello", nullptr);

        CHECK(str.str()
              == time + " INFO hello\n" + time + " ERROR hello\n" + time
                     + " ERROR hello\n");
    }
}

TEST_CASE("llama level") {
    INITIALIZE();
    ggml_log_callback logger = logging::llama_callback;

    SUBCASE("normal (info)") {
        logging::set_llama_level(logging::level::info);
        logger(GGML_LOG_LEVEL_DEBUG, "msg", nullptr);
        logger(GGML_LOG_LEVEL_ERROR, "error", nullptr);
        logger(GGML_LOG_LEVEL_DEBUG, "vsesvfdsv", nullptr);
        logger(GGML_LOG_LEVEL_INFO, "info", nullptr);

        CHECK(str.str() == time + " ERROR error\n" + time + " INFO info\n");
    }

    SUBCASE("off") {
        logging::set_llama_level(logging::level::off);
        logger(GGML_LOG_LEVEL_DEBUG, "msg", nullptr);
        logger(GGML_LOG_LEVEL_ERROR, "error", nullptr);
        logger(GGML_LOG_LEVEL_DEBUG, "vsesvfdsv", nullptr);
        logger(GGML_LOG_LEVEL_INFO, "info", nullptr);

        CHECK(str.str().empty());
    }
}

#endif

#ifdef LOGGING_RAYLIB_ADAPTER

    #include <logging/adapters/raylib.h>
    #include <raylib.h>

static void rlogger(int logLevel, const char* text, ...) {
    va_list args;
    va_start(args, text);
    logging::raylib_callback(logLevel, text, args);
    va_end(args);
}

TEST_CASE("raylib log") {
    INITIALIZE();
    TraceLogCallback logger [[maybe_unused]] = logging::raylib_callback;

    SUBCASE("trace") {
        rlogger(LOG_TRACE, "21.37", nullptr);
        CHECK(str.str() == time + " DEBUG 21.37\n");
    }

    SUBCASE("debug") {
        rlogger(LOG_DEBUG, "21.37", nullptr);
        CHECK(str.str() == time + " DEBUG 21.37\n");
    }

    SUBCASE("info") {
        rlogger(LOG_INFO, "hello", nullptr);
        CHECK(str.str() == time + " INFO hello\n");
    }

    SUBCASE("warn") {
        rlogger(LOG_WARNING, "hello", nullptr);
        CHECK(str.str() == time + " WARNING hello\n");
    }

    SUBCASE("error") {
        rlogger(LOG_ERROR, "2137", nullptr);
        CHECK(str.str() == time + " ERROR 2137\n");
    }

    SUBCASE("fatal") {
        rlogger(LOG_FATAL, "2137", nullptr);
        CHECK(str.str() == time + " FATAL 2137\n");
    }

    SUBCASE("none") {
        rlogger(LOG_NONE, "hello", nullptr);
        CHECK(str.str() == time + " INFO hello\n");
    }

    SUBCASE("all") {
        rlogger(LOG_ALL, "hello", nullptr);
        CHECK(str.str().empty());
    }

    SUBCASE("formatted") {
        rlogger(LOG_FATAL, "%c 2137 %s %d", 'a', "hello", 1234);
        CHECK(str.str() == time + " FATAL a 2137 hello 1234\n");
    }
}

TEST_CASE("raylib level") {
    INITIALIZE();

    SUBCASE("normal (info)") {
        logging::set_raylib_level(logging::level::info);
        rlogger(LOG_DEBUG, "msg");
        rlogger(LOG_ERROR, "error");
        rlogger(LOG_TRACE, "vsesvfdsv");
        rlogger(LOG_INFO, "info");

        CHECK(str.str() == time + " ERROR error\n" + time + " INFO info\n");
    }

    SUBCASE("off") {
        logging::set_raylib_level(logging::level::off);
        rlogger(LOG_DEBUG, "msg");
        rlogger(LOG_ERROR, "error");
        rlogger(LOG_TRACE, "vsesvfdsv");
        rlogger(LOG_INFO, "info");

        CHECK(str.str().empty());
    }
}


#endif

TEST_SUITE_END();
