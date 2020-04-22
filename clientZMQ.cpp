//
// Naive version of the Recommender
//
// 1. Open REQ in ZMQ
// 2. Menu for choosing a query
// 3. Prepare json according to your query
// 4. Send message to SmartMDSD via the brokerZMQ (ip/port :: tcp://127.0.0.1:5556)
//
// Request-reply based on work by Olivier Chamoux <olivier.chamoux@fr.thalesgroup.com>

#include "zhelpers.hpp"
#include "json.hpp"

std::string generate_json_get_all_positions(int uid)
{
    nlohmann::json json;
    json["msg-type"] = "query";
    json["id"] = uid;

    auto& query = json["query"];
    query["type"] = "get-all-positions";

    return json.dump(1);
}


int main (int argc, char *argv[])
{
    // 1. Open REQ in ZMQ

    zmq::context_t context(1);

    zmq::socket_t requester(context, ZMQ_REQ);
    requester.connect("tcp://127.0.0.1:5556");

    int timeout_ms = 500;
    int linger_ms = 0;
    requester.setsockopt(ZMQ_SNDTIMEO, timeout_ms);
    requester.setsockopt(ZMQ_RCVTIMEO, timeout_ms);
    requester.setsockopt(ZMQ_LINGER, linger_ms);

    int uid = 1;
    int type = 1;
    int subtype = 1;    
    int value = 100;
    std::string request_msg;

    while (1)
        {
        // 2. Menu for choosing a query

	std::cout << "********** Main Menu **********" << std::endl;
	std::cout << "(1): change_parameter" << std::endl;
	std::cout << "(2): get_all_positions" << std::endl;
	std::cout << "(3): abort_current_skill" << std::endl;
	std::cin >> type;

        if (type == 1) // Different options for change_parameter
            {

            std::cout << "*** Change_parameter Menu ***" << std::endl; // Currently only speed changes available
	    std::cout << "(1): speed change" << std::endl;
            std::cin >> subtype;

            if (subtype == 1)
                {
 
                std::cout << "***** Speed_change value *****" << std::endl; // Currently only speed changes available
	        std::cout << "speed value" << std::endl;
                std::cin >> value;
                }
            }

        // 3. Prepare json according to your query

	switch(type)
	{
	case 1:
		request_msg = generate_json_get_all_positions(uid); //_change_parameter(uid,subtype,value);
		break;
	case 2:
		request_msg = generate_json_get_all_positions(uid);
		break;
	case 3:
		request_msg = generate_json_get_all_positions(uid); //_abort_current_skill(uid);
		break;
	default:
		std::cout << "ERROR! You have selected an invalid choice." << std::endl;
		break;
	}    

        // 4. Send message to SmartMDSD via the brokerZMQ 

        zmq::message_t zmq_request_msg( request_msg.size() );
        memcpy( zmq_request_msg.data(), request_msg.c_str(), request_msg.size() );

        if( !requester.send( zmq_request_msg ) )
            {
            // timeout
            std::cout << "send TIMEOUT" << std::endl;
            return 0;
            }
        std::cout << "message sent" << std::endl;

        zmq::message_t ack;
        if( !requester.recv( &ack ) )
            {
            std::cout << "ack TIMEOUT" << std::endl;
            return 0;
            }
        std::cout << "ack received" << std::endl;

        uid++;
        }
}

