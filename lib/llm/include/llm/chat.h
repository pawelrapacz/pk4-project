#pragma once

#include <llm/common.h>
#include <llm/detail/llm_interface.h>
#include <llm/detail/vendor_fwd.h>

#include <memory>
#include <string>

namespace llm {

    class chat {
      public:
        using llm_ptr = std::unique_ptr<detail::llm_interface>;

      public:
        chat(llm_ptr, const std::string&);

        template<typename Tp, typename... Args>
            requires std::derived_from<Tp, detail::llm_interface>
        chat(Args&&... args)
            : _llm(std::make_unique<Tp>(std::forward<Args>(args)...)) { }

        const std::string& prompt(const std::string&);
        const std::string& prompt(const std::string&, const nlohmann::ordered_json&);

        const message& last_message() const;
        const chat_messages& messages() const noexcept;

      private:
        llm_ptr _llm;
        chat_messages _msg;
    };

} // namespace llm
