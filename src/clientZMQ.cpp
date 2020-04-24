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
#include "positions.hpp"
#include "change_velocity.hpp"

using namespace zmqserver;

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

    while (1){

        // 2. Menu for choosing a query
        std::cout << "********** Main Menu **********" << std::endl;
        std::cout << "(1): change_parameter" << std::endl;
        std::cout << "(2): get_all_positions" << std::endl;
        std::cout << "(3): abort_current_skill" << std::endl;
        std::cin >> type;

        // 3. Prepare json according to your query
        switch(type) 
        {
            case 1: // Different options for change_parameter
            {
                std::cout << "*** Change_parameter Menu ***" << std::endl; // Currently only speed changes available
                std::cout << "(1): speed change" << std::endl;
                std::cin >> subtype;
                switch(subtype)
                {
                    case 1: 
                    {
                        std::cout << "***** Speed_change value *****" << std::endl; // Currently only speed changes available
                        std::cout << "speed value" << std::endl;
                        std::cin >> value;
                        Velocity vel(value, 0);
                        ChangeVelocity vel_msg(uid, vel); 
                        request_msg = vel_msg.dump();
                        break;
                    }
                    default:
                        std::cout << "ERROR! You have selected an invalid choice." << std::endl;
                        break;    
                }
                break;
            }
            case 2:
            {
                Positions pos_msg(uid);
                request_msg = pos_msg.dump();
                break;
            }
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

