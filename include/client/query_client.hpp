#pragma once

#include <mutex>
#include <zmq.hpp>

class QueryClient {
  
  std::mutex mutex;
  zmq::context_t context;
  zmq::socket_t requester;
  int uid;
  std::string request_msg;

  public:
    QueryClient() :
        context(1),
        requester(context, ZMQ_REQ),
        uid(1), 
        request_msg("") {
      requester.connect("tcp://127.0.0.1:5556");
      requester.setsockopt(ZMQ_SNDTIMEO, 500);
      requester.setsockopt(ZMQ_RCVTIMEO, 500);
      requester.setsockopt(ZMQ_REQ_RELAXED, 1);
    }
    
    ~QueryClient() {
      requester.disconnect("tcp://127.0.0.1:5556");
    }

    void setMsg(std::string msg) {
	    std::lock_guard<std::mutex> lck (mutex);
      request_msg = std::move(msg);
    }

    int getID() const {
      return uid;
    }

    void send() {
      std::lock_guard<std::mutex> lck (mutex);
      zmq::message_t zmq_request_msg(request_msg.size());
      memcpy(zmq_request_msg.data(), request_msg.c_str(), request_msg.size());
      if (!requester.send(zmq_request_msg)) {
        std::cout << "send TIMEOUT" << std::endl;
        return;
      }
      std::cout << "message sent" << std::endl;
      zmq::message_t ack;
      if (!requester.recv(&ack)) {
        return;
      }
      std::cout << "ack received" << std::endl;
      uid++;
    }
};

