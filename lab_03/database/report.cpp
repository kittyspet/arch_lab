#include "report.h"
#include "database.h"

#include <sstream>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

namespace database
{
    Report Report::fromJSON(const std::string &str)
    {
        int start = str.find("_id");
        int end = str.find(",",start);

        std::string s1 = str.substr(0,start-1);
        std::string s2 = str.substr(end+1);

        Report report;

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(s1+s2);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        report.id() = object->getValue<long>("id");
        report.report_name() = object->getValue<std::string>("report_name");
        report.report_text() = object->getValue<std::string>("report_text");
        report.room_id() = object->getValue<long>("room_id");
        report.date_report() = object->getValue<std::string>("date_report");
        Poco::JSON::Object::Ptr object_array = object->getObject("comment");
        report._comment.comment_text = object_array->getValue<std::string>("comment_text");
        report._comment.commentator_id = object_array->getValue<long>("commentator_id");
        report._comment.date_comment = object_array->getValue<std::string>("date_comment");

        return report;
    }

    long Report::get_id() const
    {
        return _id;
    }

    const std::string &Report::get_report_name() const
    {
        return _report_name;
    }

    const std::string &Report::get_report_text() const
    {
        return _report_text;
    }

    long Report::get_room_id() const
    {
        return _room_id;
    }

    const std::string &Report::get_date_report() const
    {
        return _date_report;
    }

    const std::string  Report::get_comment_as_string() const
    {
        return _comment.to_string();
    }

    const std::string Report::Comment::to_string() const
    {
        std::stringstream ss;
        ss << comment_text << ", " << commentator_id << ", " << date_comment;
        return ss.str();
    }

    long &Report::id()
    {
        return _id;
    }

    std::string &Report::report_name()
    {
        return _report_name;
    }

    std::string &Report::report_text()
    {
        return _report_text;
    }

    long &Report::room_id()
    {
        return _room_id;
    }

    std::string &Report::date_report()
    {
        return _date_report;
    }

    Report::Comment &Report::comment()
    {
        return _comment;
    }

    std::optional<Report> Report::read_by_id(long id)
    {
        std::optional<Report> result;
        std::map<std::string,long> params;
        params["id"] = id;
        std::vector<std::string> results = database::Database::get().get_from_mongo("arch.reports",params);

        if(!results.empty())
            result = fromJSON(results[0]);
        
        return result;
    }

    std::vector<Report> Report::read_by_room_id(long room_id)
    {
        std::vector<Report> result;
        std::map<std::string,long> params;
        params["room_id"] = room_id;

        std::vector<std::string> results = database::Database::get().get_from_mongo("arch.reports",params);

        for(std::string& s : results) 
            result.push_back(fromJSON(s));
        

        return result;
    }

    void Report::add()
    {
        database::Database::get().send_to_mongo("reports",toJSON());
    }

    void Report::update()
    {
        std::map<std::string,long> params;
        params["id"] = _id;       
        database::Database::get().update_mongo("reports",params,toJSON());
    }

    Poco::JSON::Object::Ptr Report::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("report_name", _report_name);
        root->set("report_text", _report_text);
        root->set("room_id", _room_id);
        root->set("date_report", _date_report);

        Poco::JSON::Object::Ptr comment = new Poco::JSON::Object();

        comment->set("comment_text",_comment.comment_text);
        comment->set("commentator_id",_comment.commentator_id);
        comment->set("date_comment",_comment.date_comment);

        root->set("comment", comment);


        return root;
    }
}