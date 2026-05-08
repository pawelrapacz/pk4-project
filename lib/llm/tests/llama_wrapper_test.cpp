#include <doctest/doctest.h>
#include <llm/llama_wrapper.h>


TEST_CASE("llama.cpp wrapper") {
    llm::llama_wrapper llm(MODELS_PATH "/gemma-4-E2B-it-Q8_0.gguf",
                           {.n_gpu_layers = 0});

    std::string res
        = llm.generate("Hey! write lorem ipsum, 1 paragraph please");

    CHECK(!res.empty());
}
