/* Copyright (C) 2020 Antonio Bandera -  All Rights Reserved
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

#include "zhelpers.hpp"

int main (int argc, char *argv[])
{
    //  Prepare our context and sockets
    zmq::context_t context(1);
    zmq::socket_t frontend (context, ZMQ_ROUTER);
    zmq::socket_t backend (context, ZMQ_DEALER);

    frontend.bind("tcp://127.0.0.1:5556");
    backend.connect("tcp://localhost:5557");

    int timeout_ms = 500;
    int linger_ms = 0;
    frontend.setsockopt(ZMQ_SNDTIMEO, timeout_ms);
    frontend.setsockopt(ZMQ_RCVTIMEO, timeout_ms);
    frontend.setsockopt(ZMQ_LINGER, linger_ms);

    //  Initialize poll set
    zmq::pollitem_t items [] = {
        { static_cast<void*>(frontend), 0, ZMQ_POLLIN, 0 },
        { static_cast<void*>(backend), 0, ZMQ_POLLIN, 0 }
    };
    
    //  Switch messages between sockets
    while (1) {
        zmq::message_t message;
        int more;               //  Multipart detection

        zmq::poll (&items [0], 2, -1);
        
        if (items [0].revents & ZMQ_POLLIN) {
            while (1) {
                //  Process all parts of the message
                frontend.recv(&message);
                
                // Debug - checking messages from the BT to SmartMDSD
                std::string msg;
                msg = std::string(static_cast<char*>(message.data()), message.size());
                std::cout << "msg_(from BT) :: " << msg << std::endl;

                size_t more_size = sizeof (more);
                frontend.getsockopt(ZMQ_RCVMORE, &more, &more_size);
                backend.send(message, more? ZMQ_SNDMORE: 0);                
                if (!more)
                    break;      //  Last message part
            }
        }
        if (items [1].revents & ZMQ_POLLIN) {
            while (1) {
                //  Process all parts of the message
                backend.recv(&message);

                size_t more_size = sizeof (more);
                backend.getsockopt(ZMQ_RCVMORE, &more, &more_size);
                frontend.send(message, more? ZMQ_SNDMORE: 0);
                if (!more)
                    break;      //  Last message part
            }
        }
    }
    return 0;
}
