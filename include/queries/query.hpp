#pragma once

#include "json.hpp"

namespace zmqserver{

class Query
{
    protected:
        int _id;
        std::string _msg_type;
        nlohmann::json _json;
        nlohmann::json _query;

    public:
        Query(int id) : 
            _id(id),
            _msg_type("query")
        {
            _json["msg-type"] = _msg_type;
            _json["id"] = _id;
        };
        ~Query(){};
        
        virtual std::string dump(){
            _json["query"] = _query;
            return _json.dump(1);
        }
};
}