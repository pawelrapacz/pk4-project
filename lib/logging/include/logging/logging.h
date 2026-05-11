#pragma once

#include <logging/styles.h>

#include <array>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>


// configuration macros:
// LOGGING_DISABLE
// LOGGING_NO_COLORS


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

#ifndef LOGGING_DISABLE

    namespace detail {

        struct specs {
            level logLevel             = level {};
            bool toFile                = false;
            bool toStdout              = true;
            std::ostream* stream       = &default_stream;
            std::filesystem::path path = "logs/program.log";
            std::mutex fileMtx;
            std::mutex streamMtx;

          private:
            std::ofstream file;
            friend inline std::ofstream& get_log_file() noexcept;
        };

        inline specs log_specs {};

        inline std::ostream& get_log_stream() noexcept {
            return *log_specs.stream;
        }

        inline std::ofstream& get_log_file() noexcept { return log_specs.file; }

        inline constexpr styles::style get_style(level lvl) noexcept {
            using namespace styles;
            static constexpr auto level_styles = std::to_array<style>({
                FG_BLUE | ITALIC, // DEBUG
                {},               // INFO
                FG_YELLOW,        // WARN
                FG_RED | BOLD,    // ERROR
                BG_RED | BOLD,    // FATAL
                {},               // OFF
            });

            return level_styles[static_cast<std::size_t>(lvl)];
        }

        inline bool should_flush(level lvl) noexcept {
            return lvl > level::warn;
        }

        inline constexpr std::string_view get_level_name(level lvl) noexcept {
            static constexpr auto level_names
                = std::to_array<std::string_view>({
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
        inline std::string create_log_message(level lvl, const Tp& msg) {
            auto time = std::chrono::zoned_time(
                std::chrono::current_zone(), std::chrono::system_clock::now());
            return std::format("[{:%F %X}] {} {}", time, get_level_name(lvl),
                               msg);
        }

        inline void write_to_console(level lvl, const std::string& msg) {
            std::lock_guard lock(log_specs.streamMtx);

    #ifndef LOGGING_NO_COLORS
            get_log_stream() << styles::apply_seq(get_style(lvl)) << msg
                             << styles::reset_seq() << '\n';
    #else
            get_log_stream() << msg << "\n";
    #endif
            if (should_flush(lvl)) get_log_stream().flush();
        }

        inline void write_to_file(level lvl, const std::string& msg) {
            std::lock_guard lock(log_specs.fileMtx);

            if (!get_log_file().is_open()) {
                if (log_specs.path.has_parent_path())
                    std::filesystem::create_directories(
                        log_specs.path.parent_path());

                get_log_file().open(log_specs.path);
            }

            get_log_file() << msg << '\n';

            if (should_flush(lvl)) get_log_file().flush();
        }

    } // namespace detail


    inline void set_level(level lvl) noexcept {
        detail::log_specs.logLevel = lvl;
    }

    inline void set_file(const std::filesystem::path& path) {
        assert(!detail::get_log_file().is_open()
               && "The file can not be changed after it has been opend (after "
                  "the first log)");
        detail::log_specs.path = path;
    }

    inline void set_stdout(std::ostream& stream) {
        detail::log_specs.stream = &stream;
    }

    inline void to_file(bool tof = true) noexcept {
        detail::log_specs.toFile = tof;
    }

    inline void to_stdout(bool tos = true) noexcept {
        detail::log_specs.toStdout = tos;
    }


    template<typename Tp>
    inline void log(level lvl, const Tp& msg) {
        // omit logs below the desired level
        if (lvl < detail::log_specs.logLevel) return;

        std::string logMsg = detail::create_log_message(lvl, msg);

        if (detail::log_specs.toStdout) detail::write_to_console(lvl, logMsg);

        if (detail::log_specs.toFile) detail::write_to_file(lvl, logMsg);
    }

    template<typename Tp>
    inline void debug(const Tp& msg [[maybe_unused]]) {
        log(level::debug, msg);
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
    inline void log(level lvl, std::format_string<Args...> fmt,
                    Args&&... args) {
        log(lvl, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    inline void debug(std::format_string<Args...> fmt, Args&&... args) {
        log(level::debug, fmt, std::forward<Args>(args)...);
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

#else  // LOGGING_DISABLE

    inline void set_level(level) noexcept { }

    inline void set_file(const std::filesystem::path&) { }

    inline void set_stdout(std::ostream&) { }

    inline void to_file(bool = true) noexcept { }

    inline void to_stdout(bool = true) noexcept { }


    template<typename Tp>
    inline void log(level, const Tp&) { }

    template<typename Tp>
    inline void debug(const Tp&) { }

    template<typename Tp>
    inline void info(const Tp&) { }

    template<typename Tp>
    inline void warn(const Tp&) { }

    template<typename Tp>
    inline void error(const Tp&) { }

    template<typename Tp>
    inline void fatal(const Tp&) { }


    template<typename... Args>
    inline void log(level, std::format_string<Args...>, Args&&...) { }

    template<typename... Args>
    inline void debug(std::format_string<Args...>, Args&&...) { }

    template<typename... Args>
    inline void info(std::format_string<Args...>, Args&&...) { }

    template<typename... Args>
    inline void warn(std::format_string<Args...>, Args&&...) { }

    template<typename... Args>
    inline void error(std::format_string<Args...>, Args&&...) { }

    template<typename... Args>
    inline void fatal(std::format_string<Args...>, Args&&...) { }

#endif // LOGGING_DISABLE

} // namespace logging
