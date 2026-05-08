#pragma once

#include <llama.h>
#include <nlohmann/json_fwd.hpp>

#include <atomic>
#include <filesystem>
#include <string>
#include <vector>

namespace llm {

    class llama_wrapper {
      public:
        struct params {
            uint32_t n_ctx       = 512;
            int32_t n_gpu_layers = -1;
        };

      public:
        llama_wrapper(const std::filesystem::path&);
        llama_wrapper(const std::filesystem::path&, params);

        ~llama_wrapper();

        std::string chat(const std::string&);
        std::string generate(const std::string&);
        std::string generate(const std::string&, const nlohmann::json&);

      private:
        static void backend_init();
        static void backend_free();

        std::string generate_impl(const std::string&, llama_sampler*);
        std::vector<llama_token> tokenize(const std::string&,
                                          const llama_vocab*) const;


      private:
        static inline std::atomic_uint32_t _s_backend_refcount = 0;

      private:
        llama_model* _model     = nullptr;
        llama_context* _context = nullptr;
    };

} // namespace llm
