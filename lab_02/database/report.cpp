#include "report.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void Report::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS reports (id SERIAL,"
                        << "report_name VARCHAR(256) NOT NULL,"
                        << "text VARCHAR(256) NOT NULL,"
                        << "room INT NULL);",
                now;
        }

        catch (Poco::Data::PostgreSQL::PostgreSQLException &e)
        {
            std::cout << "connection:" << e.displayText() << std::endl;
            throw;
        }
        catch (Poco::Data::ConnectionFailedException &e)
        {
            std::cout << "connection:" << e.displayText() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Report::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("report_name", _report_name);
        root->set("room", _room);
        root->set("text", _text);

        return root;
    }

    Report Report::fromJSON(const std::string &str)
    {
        Report report;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        report.id() = object->getValue<long>("id");
        report.report_name() = object->getValue<std::string>("report_name");
        report.room() = std::stol(object->getValue<std::string>("room"));
        report.text() = object->getValue<std::string>("text");

        return report;
    }

    std::optional<Report> Report::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Report a;
            select << "SELECT id, report_name, room,text FROM reports where id=$1",
                into(a._id),
                into(a._report_name),
                into(a._room),
                into(a._text),
                use(id),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst())
                return a;
        }

        catch (Poco::Data::PostgreSQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::PostgreSQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
        }
        return {};
    }


    std::vector<Report> Report::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Report> result;
            Report a;
            select << "SELECT id, report_name, room, text FROM reports",
                into(a._id),
                into(a._report_name),
                into(a._room),
                into(a._text),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::PostgreSQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::PostgreSQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::vector<Report> Report::search(std::string report_name)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Report> result;
            Report a;
            report_name += "%";
            select << "SELECT id, report_name, room, text FROM reports where report_name LIKE $1",
                into(a._id),
                into(a._report_name),
                into(a._room),
                into(a._text),
                use(report_name),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::PostgreSQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::PostgreSQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    void Report::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO reports (report_name,room,text) VALUES($1, $2, $3)",
                use(_report_name),
                use(_room),
                use(_text);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LASTVAL()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
        }
        catch (Poco::Data::PostgreSQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::PostgreSQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }


    const std::string &Report::get_text() const
    {
        return _text;
    }


    std::string &Report::text()
    {
        return _text;
    }

    long Report::get_id() const
    {
        return _id;
    }

    const std::string &Report::get_report_name() const
    {
        return _report_name;
    }


    long Report::get_room() const
    {
        return _room;
    }



    long &Report::id()
    {
        return _id;
    }

    std::string &Report::report_name()
    {
        return _report_name;
    }



    long &Report::room()
    {
        return _room;
    }

}