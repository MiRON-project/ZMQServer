#ifndef QUERY_POSITIONS_H
#define QUERY_POSITIONS_H

#include "json.hpp"
#include "query.hpp"

namespace zmqserver{

class Positions : public Query
{
    public:
        Positions(int id) : 
            Query(id)
        {
            _query["type"] = "get-all-positions";
        }
        ~Positions(){};
};

}

#endif