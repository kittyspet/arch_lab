#pragma once

#include <string>
#include <vector>
#include <optional>

#include <Poco/JSON/Object.h>

namespace database
{
    class Report{
        private:
            long _id;
            std::string _report_name;
            std::string _report_text;
            long _room_id;
            std::string _date_report;
            struct Comment {
                std::string comment_text;
                long commentator_id;
                std::string date_comment;
                const std::string to_string() const;
            };
            Comment _comment;


        public:

            static Report fromJSON(const std::string & str);

            long               get_id() const;
            const std::string &get_report_name() const;
            const std::string &get_report_text() const;
            long               get_room_id() const;
            const std::string &get_date_report() const;
            const std::string  get_comment_as_string() const;

            long&        id();
            std::string& report_name();
            std::string& report_text();
            long&        room_id();
            std::string& date_report();
            Comment&     comment();

            static std::optional<Report> read_by_id(long id);
            static std::vector<Report> read_by_room_id(long room_id);
            void add();
            void update();
            Poco::JSON::Object::Ptr toJSON() const;

    };
}