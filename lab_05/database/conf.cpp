#include "conf.h"
#include "database.h"

#include <sstream>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

namespace database
{
    Conf Conf::fromJSON(const std::string &str)
    {
        int start = str.find("_id");
        int end = str.find(",",start);

        std::string s1 = str.substr(0,start-1);
        std::string s2 = str.substr(end+1);

        Conf conf;

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(s1+s2);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        conf.id() = object->getValue<long>("id");
        conf.conf_name() = object->getValue<std::string>("conf_name");
        conf.conf_password() = object->getValue<std::string>("conf_password");
        conf.owner_id() = object->getValue<long>("owner_id");
        conf.date_create_conf() = object->getValue<std::string>("date_create_conf");
        
        return conf;
    }

    long Conf::get_id() const
    {
        return _id;
    }

    const std::string &Conf::get_conf_name() const
    {
        return _conf_name;
    }

    const std::string &Conf::get_conf_password() const
    {
        return _conf_password;
    }

    long Conf::get_owner_id() const
    {
        return _owner_id;
    }

    const std::string &Conf::get_date_create_conf() const
    {
        return _date_create_conf;
    }



    long &Conf::id()
    {
        return _id;
    }

    std::string &Conf::conf_name()
    {
        return _conf_name;
    }

    std::string &Conf::conf_password()
    {
        return _conf_password;
    }

    long &Conf::owner_id()
    {
        return _owner_id;
    }

    std::string &Conf::date_create_conf()
    {
        return _date_create_conf;
    }


    std::optional<Conf> Conf::read_by_id(long id)
    {
        std::optional<Conf> result;
        std::map<std::string,long> params;
        params["id"] = id;
        std::vector<std::string> results = database::Database::get().get_from_mongo("arch.confs",params);

        if(!results.empty())
            result = fromJSON(results[0]);
        
        return result;
    }

    std::vector<Conf> Conf::read_by_owner_id(long owner_id)
    {
        std::vector<Conf> result;
        std::map<std::string,long> params;
        params["owner_id"] = owner_id;

        std::vector<std::string> results = database::Database::get().get_from_mongo("arch.confs",params);

        for(std::string& s : results) 
            result.push_back(fromJSON(s));
        

        return result;
    }

    void Conf::add()
    {
        database::Database::get().send_to_mongo("confs",toJSON());
    }

    void Conf::update()
    {
        std::map<std::string,long> params;
        params["id"] = _id;       
        database::Database::get().update_mongo("confs",params,toJSON());
    }

    Poco::JSON::Object::Ptr Conf::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("conf_name", _conf_name);
        root->set("conf_password", _conf_password);
        root->set("owner_id", _owner_id);
        root->set("date_create_conf", _date_create_conf);



        return root;
    }
}