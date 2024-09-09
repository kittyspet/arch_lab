#include "conf.h"
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

    void Conf::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS confs (id SERIAL,"
                        << "conf_name VARCHAR(256) NOT NULL,"
                        << "password VARCHAR(256) NOT NULL,"
                        << "owner INT NULL);",
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

    Poco::JSON::Object::Ptr Conf::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("conf_name", _conf_name);
        root->set("owner", _owner);
        root->set("password", _password);

        return root;
    }

    Conf Conf::fromJSON(const std::string &str)
    {
        Conf conf;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        conf.id() = object->getValue<long>("id");
        conf.conf_name() = object->getValue<std::string>("conf_name");
        conf.owner() = std::stol(object->getValue<std::string>("owner"));
        conf.password() = object->getValue<std::string>("password");

        return conf;
    }

    std::optional<Conf> Conf::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Conf a;
            select << "SELECT id, conf_name, owner,password FROM confs where id=$1",
                into(a._id),
                into(a._conf_name),
                into(a._owner),
                into(a._password),
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


    std::vector<Conf> Conf::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Conf> result;
            Conf a;
            select << "SELECT id, conf_name, owner, password FROM confs",
                into(a._id),
                into(a._conf_name),
                into(a._owner),
                into(a._password),
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

    std::vector<Conf> Conf::search(std::string conf_name)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Conf> result;
            Conf a;
            conf_name += "%";
            select << "SELECT id, conf_name, owner, password FROM confs where conf_name LIKE $1",
                into(a._id),
                into(a._conf_name),
                into(a._owner),
                into(a._password),
                use(conf_name),
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

    void Conf::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO confs (conf_name,owner,password) VALUES($1, $2, $3, $4)",
                use(_conf_name),
                use(_owner),
                use(_password);

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


    const std::string &Conf::get_password() const
    {
        return _password;
    }


    std::string &Conf::password()
    {
        return _password;
    }

    long Conf::get_id() const
    {
        return _id;
    }

    const std::string &Conf::get_conf_name() const
    {
        return _conf_name;
    }


    const long &Conf::get_owner() const
    {
        return _owner;
    }



    long &Conf::id()
    {
        return _id;
    }

    std::string &Conf::conf_name()
    {
        return _conf_name;
    }



    long &Conf::owner()
    {
        return _owner;
    }

}