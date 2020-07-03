/* Copyright (C) 2020 Renan Freitas, Blue Ocean Robotics -  All Rights Reserved
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
*   to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
*   and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

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