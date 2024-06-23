#pragma once
#include "tagged.h"

#include <sstream>
#include <random>
#include <map>
#include <memory>

namespace detail  {
	struct TokenTag {};
}  // namespace detail


namespace app {
    using Token = util::Tagged<std::string, detail::TokenTag>;
    using UserSessionId = util::Tagged<std::string, uint64_t>;

    class UserTokens {
    public:
   
        std::string GetToken();
        
    private:
        std::random_device random_device_;
        std::mt19937_64 generator1_{[this] {
            std::uniform_int_distribution<std::mt19937_64::result_type> dist;
            return dist(random_device_);
        }()};
        std::mt19937_64 generator2_{[this] {
            std::uniform_int_distribution<std::mt19937_64::result_type> dist;
            return dist(random_device_);
        }()};
    }; 

    class User {
    public:
        User(uint64_t id, UserSessionId User_session) : id_(id), User_session_(User_session) {};

        const UserSessionId& GetUserSessionId() const noexcept {
            return User_session_;
        }

        const uint64_t& GetId() const {
            return id_;
        }

    private:
        uint64_t id_;
        UserSessionId User_session_;
    };

    class Users
    {
    public:

        const std::string AddUser(const uint64_t id, const UserSessionId& User_session);

        std::shared_ptr<User> FindByToken(std::string& str) const;

        std::shared_ptr<User> FindByToken(Token& token) const;

        const Token FindTokenByUserId(uint64_t id) const;

        const Token FindTokenByName(const std::string& name) const;

        const std::map<Token, std::shared_ptr<User>>& GetUsers() const noexcept
        {
            return Users_;
        }

    private:
        uint64_t count_Users_ = 0;
        std::map<Token, std::shared_ptr<User>> Users_;
        
    };     
}