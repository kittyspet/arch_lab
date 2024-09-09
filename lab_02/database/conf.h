#pragma once

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Conf{
        private:
            long _id;
            std::string _conf_name;
            long _owner;
            std::string _password;

        public:

            static Conf fromJSON(const std::string & str);

            long             get_id() const;
            const std::string &get_conf_name() const;
            const long &get_owner() const;
            const std::string &get_password() const;

            long&        id();
            std::string &conf_name();
            long &owner();
            std::string &password();

            static void init();
            static std::optional<Conf> read_by_id(long id);
            static std::vector<Conf> read_all();
            static std::vector<Conf> search(std::string conf_name);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

