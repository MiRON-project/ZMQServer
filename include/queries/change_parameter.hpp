#pragma once

#include "json.hpp"
#include "query.hpp"

namespace zmqserver{

template <class T>
class ChangeParameter : public Query
{
    protected:
        T _values;
    public:
        ChangeParameter(int id, T values) : 
            Query(id),
            _values(values)
        {
            _query["type"] = "change-parameter";
        }
        ~ChangeParameter(){};
        
        void setParameter(const T& values){_values = values;};
};

}