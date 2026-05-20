#pragma once

#include <llm/common.h>
#include <llm/detail/exception.h>
#include <llm/detail/llm_interface.h>
#include <llm/detail/vendor_fwd.h>

#include <nlohmann/json_fwd.hpp>

#include <atomic>
#include <filesystem>
#include <string>

namespace llm {

    class llama_wrapper : public detail::llm_interface {
      public:
        struct params {
            uint32_t n_ctx       = 512;
            int32_t n_gpu_layers = -1;
        };

      public:
        llama_wrapper(const std::filesystem::path&);
        llama_wrapper(const std::filesystem::path&, params);

        llama_wrapper(llama_wrapper&&) noexcept;
        llama_wrapper& operator=(llama_wrapper&&) noexcept;

        llama_wrapper(const llama_wrapper&)            = delete;
        llama_wrapper& operator=(const llama_wrapper&) = delete;

        ~llama_wrapper();

        std::string& chat(chat_messages&) override;
        std::string& chat(chat_messages&,
                          const nlohmann::ordered_json&) override;
        std::string generate(const std::string&) override;
        std::string generate(const std::string&,
                             const nlohmann::ordered_json&) override;

      private:
        static void backend_init();
        static void backend_free();

        llama_sampler* sampler() const;
        llama_sampler* json_grammar_sampler(const nlohmann::ordered_json&) const;

        std::string generate_impl(const std::string&, llama_sampler*);
        std::string& chat_impl(chat_messages&, llama_sampler*);

      private:
        static inline std::atomic_uint32_t _s_backend_refcount = 0;

      private:
        llama_model* _model     = nullptr;
        llama_context* _context = nullptr;
    };


    class llama_wrapper_error : public detail::exception {
      public:
        explicit llama_wrapper_error(const char* msg)
            : exception(msg) { }

        explicit llama_wrapper_error(const std::string& msg)
            : exception(msg) { }

        llama_wrapper_error(llama_wrapper_error&&) noexcept = default;
        llama_wrapper_error& operator=(llama_wrapper_error&&) noexcept
            = default;
        llama_wrapper_error(const llama_wrapper_error&)            = default;
        llama_wrapper_error& operator=(const llama_wrapper_error&) = default;

        virtual ~llama_wrapper_error() = default;
    };


} // namespace llm
