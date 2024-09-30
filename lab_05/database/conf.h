#pragma once

#include <string>
#include <vector>
#include <optional>

#include <Poco/JSON/Object.h>

namespace database
{
    class Conf{
        private:
            long _id;
            std::string _conf_name;
            std::string _conf_password;
            long _owner_id;
            std::string _date_create_conf;


        public:

            static Conf fromJSON(const std::string & str);

            long               get_id() const;
            const std::string &get_conf_name() const;
            const std::string &get_conf_password() const;
            long               get_owner_id() const;
            const std::string &get_date_create_conf() const;

            long&        id();
            std::string& conf_name();
            std::string& conf_password();
            long&        owner_id();
            std::string& date_create_conf();

            static std::optional<Conf> read_by_id(long id);
            static std::vector<Conf> read_by_owner_id(long owner_id);
            void add();
            void update();
            Poco::JSON::Object::Ptr toJSON() const;

    };
}