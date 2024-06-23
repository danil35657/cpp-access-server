#include "users.h"
#include <iomanip>
#include <iostream>
#include <memory>

namespace app {
    static int32_t index = 0;

    std::string UserTokens::GetToken() {
        std::ostringstream ss;
        ss << std::hex << generator1_() << generator2_();
        return ss.str();
    }

    const std::string Users::AddUser(uint64_t id, const UserSessionId& User_session) {
        try {
            auto user_id_tag_ = util::Tagged<std::string, uint64_t>(id);
            std::string token;
            
            do {
                token.clear();
                token = std::make_unique<UserTokens>()->GetToken();
            } while (token.size() != 32);

            
            auto new_User_ = std::make_shared<User>(++count_Users_, std::move(user_id_tag_), User_session);

            auto pair = std::make_pair<Token, std::shared_ptr<User>>(util::Tagged<std::string, detail::TokenTag>(token), std::move(new_User_));
            Users_.insert(pair);

            return token;
        }
        catch (...) {
            return std::string();
        }

        return std::string();
    }

    std::shared_ptr<User> Users::FindByToken(std::string &str) const {
        Token token = util::Tagged<std::string, detail::TokenTag>(str);
        return Users_.at(token);
    }

    std::shared_ptr<User> Users::FindByToken(Token &token) const {
        return Users_.at(token);
    }

    const Token Users::FindTokenByUserId(uint64_t id) const {
        for (const auto &item : Users_)
        {
            if (item.second.get()->GetId() == id)
                return item.first;
        }
    }

    const Token Users::FindTokenByName(const std::string &name) const {
        auto name_tag_ = util::Tagged<std::string, uint64_t>(name);
        for (const auto &item : Users_) {
            if (item.second.get()->GetUserId() == name_tag_)
                return item.first;
        }
    }
}