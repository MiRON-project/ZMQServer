#ifndef QUERY_CHANGE_VELOCITY_H
#define QUERY_CHANGE_VELOCITY_H

#include "json.hpp"
#include "change_parameter.hpp"

namespace zmqserver{

struct Velocity{
    std::unordered_map<std::string, double> values;

    Velocity(double vel, double omega)
    {
        values.insert(std::make_pair("1", vel));
        values.insert(std::make_pair("2", omega));
    }

    Velocity()
    {
        values.insert(std::make_pair("1", 0.0));
        values.insert(std::make_pair("2", 0.0));
    }
};


class ChangeVelocity : public ChangeParameter<Velocity>
{
    public:
        ChangeVelocity(int id, Velocity values) : 
            ChangeParameter<Velocity>(id, values)
        {
            _query["ParameterSetRepository"] = "CommNavigationObjects";
            _query["ParameterSet"] = "CdlParameter";
            _query["Component"] = "SmartCdlServer";
            _query["Parameter"] = "transvel";
        }
        ~ChangeVelocity(){};
        
        std::string dump() override{
            nlohmann::json values;
            
            values["1"] = _values.values["1"];
            values["2"] = _values.values["2"];
            _query["values"] = values;
            return Query::dump();
        }
};

}

#endif