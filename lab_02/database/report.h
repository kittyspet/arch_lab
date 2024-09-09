#pragma once

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Report{
        private:
            long _id;
            std::string _report_name;
            long _room;
            std::string _text;

        public:

            static Report fromJSON(const std::string & str);

            long             get_id() const;
            const std::string &get_report_name() const;
            long             get_room() const;
            const std::string &get_text() const;

            long&        id();
            std::string &report_name();
            long&        room();
            std::string &text();

            static void init();
            static std::optional<Report> read_by_id(long id);
            static std::vector<Report> read_all();
            static std::vector<Report> search(std::string report_name);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

