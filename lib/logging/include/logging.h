#pragma once

#include <iostream>
#include <format>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>

#include "styles.h"


#if !defined(NDEBUG) || defined(_DEBUG) || defined(LOGGING_ALWAYS_USE_DEBUG)
    #define LOGGING_USE_DEBUG_
#endif

namespace logging {

    enum class level {
        debug,
        info,
        warn,
        error,
        fatal,
        off,
    };

    inline auto& default_stream = std::clog;

    inline struct {
        level level = level::debug;
        bool toFile = false;
        bool toStdout = true;
        std::ostream* stream = &std::clog;
        std::filesystem::path path = "logs/program.log";

    protected:
        std::ofstream file;
        friend inline std::ofstream& get_log_file_() noexcept;
    } log_specs_;


    inline void set_level(level lvl) noexcept {
        log_specs_.level = lvl;
    }

    inline void to_file(bool tof = true) noexcept {
        log_specs_.toFile = tof;
    }

    inline void to_stdout(bool tos = true) noexcept {
        log_specs_.toStdout = tos;
    }

    inline void set_file(const std::filesystem::path& path) {
        log_specs_.path = path;
    }

    inline void set_stdout(std::ostream& stream) {
        log_specs_.stream = &stream;
    }

    inline std::ostream& get_log_stream_() noexcept {
        return *log_specs_.stream;
    }

    inline std::ofstream& get_log_file_() noexcept {
        return log_specs_.file;
    }

    inline constexpr styles::style get_style_(level lvl) noexcept {
        using namespace styles;
        static constexpr auto level_styles = std::to_array<style>({
            FG_BLUE | ITALIC,   // DEBUG
            {},                 // INFO
            FG_YELLOW,          // WARN
            FG_RED | BOLD,      // ERROR
            BG_RED | BOLD,      // FATAL
            {},                 // OFF
        });

        return level_styles[static_cast<std::size_t>(lvl)];
    }

    inline constexpr std::string_view get_level_name_(level lvl) noexcept {
        static constexpr auto level_names = std::to_array<std::string_view>({
            "DEBUG",
            "INFO",
            "WARNING",
            "ERROR",
            "FATAL",
            {}, // OFF
        });

        return level_names[static_cast<std::size_t>(lvl)];
    }

    template<typename Tp>
    inline std::string create_log_message_(level lvl, const Tp& msg) {
        auto time = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now());
        return std::format("[{:%F %X}] {} {}", time, get_level_name_(lvl), msg);
    }

    inline void write_to_console_(styles::style style [[maybe_unused]], const std::string& msg) {
#ifndef LOGGING_NO_COLORS
        get_log_stream_() << styles::apply_seq(style) << msg << styles::reset_seq() << '\n';
#else
        get_log_stream_() << msg << "\n";
#endif
        get_log_stream_().flush();
    }

    inline void write_to_file_(const std::string& msg) {
        if (!get_log_file_().is_open()) {
            if (log_specs_.path.has_parent_path())
                std::filesystem::create_directories(log_specs_.path.parent_path());
            
            get_log_file_().open(log_specs_.path);
        }

        get_log_file_() << msg << '\n';
        get_log_file_().flush();
    }


    template<typename Tp>
    inline void log(level lvl, const Tp& msg) {
        // omit logs below the desired level
        if (lvl < log_specs_.level) return;

        std::string logMsg = create_log_message_(lvl, msg);

        if (log_specs_.toStdout)
            write_to_console_(get_style_(lvl), logMsg);

        if (log_specs_.toFile)
            write_to_file_(logMsg);
    }

    template<typename Tp>
    inline void debug(const Tp& msg [[maybe_unused]]) {
#ifdef LOGGING_USE_DEBUG_
        log(level::debug, msg);
#endif
    }

    template<typename Tp>
    inline void info(const Tp& msg) {
        log(level::info, msg);
    }

    template<typename Tp>
    inline void warn(const Tp& msg) {
        log(level::warn, msg);
    }

    template<typename Tp>
    inline void error(const Tp& msg) {
        log(level::error, msg);
    }

    template<typename Tp>
    inline void fatal(const Tp& msg) {
        log(level::fatal, msg);
    }

    template<typename... Args>
    inline void log(level lvl, std::format_string<Args...> fmt, Args&&... args) {
        log(lvl, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    inline void debug(std::format_string<Args...> fmt [[maybe_unused]], Args&&... args [[maybe_unused]]) {
#ifdef LOGGING_USE_DEBUG_
        log(level::debug, fmt, std::forward<Args>(args)...);
#endif
    }

    template<typename... Args>
    inline void info(std::format_string<Args...> fmt, Args&&... args) {
        log(level::info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline void warn(std::format_string<Args...> fmt, Args&&... args) {
        log(level::warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline void error(std::format_string<Args...> fmt, Args&&... args) {
        log(level::error, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline void fatal(std::format_string<Args...> fmt, Args&&... args) {
        log(level::fatal, fmt, std::forward<Args>(args)...);
    }

} // logging
