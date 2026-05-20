#include "llm/llama_wrapper.h"
#include "llm/common.h"

#include <common.h>
#include <json-schema-to-grammar.h>
#include <llama.h>
#include <logging/adapters/llama.h>
#include <logging/logging.h>
#include <nlohmann/json.hpp>

#include <filesystem>
#include <mutex>
#include <string>
#include <sys/types.h>
#include <vector>

using namespace llm;


void llama_wrapper::backend_init() {
    static std::once_flag once;
    std::call_once(once, llama_log_set, logging::llama_callback, nullptr);

    if (_s_backend_refcount.fetch_add(1) == 0) llama_backend_init();
}

void llama_wrapper::backend_free() {
    if (_s_backend_refcount.fetch_sub(1) == 1) llama_backend_free();
}


llama_wrapper::llama_wrapper(const std::filesystem::path& path)
    : llama_wrapper(path, params {}) { }

llama_wrapper::llama_wrapper(const std::filesystem::path& path, params params) {
    backend_init();

    llama_model_params mdl_params = llama_model_default_params();
    mdl_params.n_gpu_layers       = params.n_gpu_layers;

    _model = llama_model_load_from_file(path.string().c_str(), mdl_params);

    if (!_model) throw llama_wrapper_error("could not load model");

    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx                = params.n_ctx;

    _context = llama_init_from_model(_model, ctx_params);
}


llama_wrapper::llama_wrapper(llama_wrapper&& other) noexcept
    : _model(other._model), _context(other._context) {
    other._model   = nullptr;
    other._context = nullptr;
}


llama_wrapper& llama_wrapper::operator=(llama_wrapper&& other) noexcept {
    _model         = other._model;
    _context       = other._context;
    other._model   = nullptr;
    other._context = nullptr;
    return *this;
}


llama_wrapper::~llama_wrapper() {
    if (_context) llama_free(_context);

    if (_model) llama_model_free(_model);

    backend_free();
}


llama_sampler* llama_wrapper::sampler() const {
    llama_sampler* smpl = llama_sampler_chain_init(llama_sampler_chain_default_params());
    llama_sampler_chain_add(smpl, llama_sampler_init_min_p(0.05f, 1));
    llama_sampler_chain_add(smpl, llama_sampler_init_temp(0.8f));
    llama_sampler_chain_add(smpl, llama_sampler_init_dist(LLAMA_DEFAULT_SEED));
    return smpl;
}

llama_sampler* llama_wrapper::json_grammar_sampler(const nlohmann::ordered_json& format) const {
    auto vocab = llama_model_get_vocab(_model);
    auto grammar = json_schema_to_grammar(format);

    llama_sampler* smpl = llama_sampler_chain_init(llama_sampler_chain_default_params());
    llama_sampler_chain_add(smpl, llama_sampler_init_grammar(vocab, grammar.c_str(), "root"));
    llama_sampler_chain_add(smpl, llama_sampler_init_greedy());
    return smpl;
}

std::string llama_wrapper::generate(const std::string& prompt) {
    return generate_impl(prompt, sampler());
}

std::string llama_wrapper::generate(const std::string& prompt,
                                    const nlohmann::ordered_json& format) {
    return generate_impl(prompt, json_grammar_sampler(format));
}

std::string& llama_wrapper::chat(chat_messages& msg) { return chat_impl(msg, sampler()); }

std::string& llama_wrapper::chat(chat_messages& msg, const nlohmann::ordered_json& format) {
    return chat_impl(msg, json_grammar_sampler(format));
}

std::string llama_wrapper::generate_impl(const std::string& prompt,
                                         llama_sampler* smpl) {
    std::string response;

    const llama_vocab* vocab = llama_model_get_vocab(_model);
    bool is_first
        = llama_memory_seq_pos_max(llama_get_memory(_context), 0) == -1;
    auto tokens = common_tokenize(_context, prompt, is_first, true);

    llama_batch batch = llama_batch_get_one(tokens.data(), tokens.size());
    llama_token new_token_id;

    while (true) {
        int32_t n_ctx = llama_n_ctx(_context);
        int32_t n_ctx_used
            = llama_memory_seq_pos_max(llama_get_memory(_context), 0) + 1;

        if (n_ctx_used + batch.n_tokens > n_ctx)
            throw llama_wrapper_error("context size exceded");

        int ret = llama_decode(_context, batch);
        if (ret != 0) throw llama_wrapper_error("failed to decode");

        new_token_id = llama_sampler_sample(smpl, _context, -1);

        if (llama_vocab_is_eog(vocab, new_token_id)) break;

        char buf[256];
        int32_t n = llama_token_to_piece(vocab, new_token_id, buf, sizeof(buf),
                                         0, true);

        if (n < 0)
            throw llama_wrapper_error("failed to convert token to piece");

        response.append(buf, n);

        batch = llama_batch_get_one(&new_token_id, 1);
    }

    llama_sampler_free(smpl);
    return response;
}


std::string& llama_wrapper::chat_impl(chat_messages& msg, llama_sampler* smpl) {
    // const char* tmpl = llama_model_chat_template(_model, nullptr);
    // if (!tmpl)
    //     throw llama_wrapper_error("failed to load chat template");

    // std::vector<llama_chat_message> llama_msg;
    // std::size_t all_msg_length = 0;
    // for (auto& m : msg) {
    //     llama_msg.push_back({m.role.c_str(), m.content.c_str()});
    //     all_msg_length += m.content.length();
    // }

    // // add the user input to the message list and format it
    // std::vector<char> formatted(all_msg_length * 2);

    // int32_t new_len = llama_chat_apply_template(tmpl, llama_msg.data(), llama_msg.size(), true,
    // formatted.data(), formatted.size()); if (new_len > (int32_t)formatted.size()) {
    //     formatted.resize(new_len);
    //     new_len = llama_chat_apply_template(tmpl, llama_msg.data(), llama_msg.size(), true,
    //     formatted.data(), formatted.size());
    // }

    // if (new_len < 0)
    //     throw llama_wrapper_error("failed to apply chat template");

    std::string res = generate_impl(msg.back().content, smpl);

    msg.emplace_back("asisstant", res);
    return msg.back().content;
}
