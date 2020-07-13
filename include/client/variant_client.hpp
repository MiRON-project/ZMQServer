/*
    Copyright (C) 2020  University of Extremadura, University of Málaga, Blue Ocean Robotics.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

		Author: Renan Freitas, renan028@gmail.com
		Maintainer: Renan Freitas, renan028@gmail.com
*/

#pragma once

#include "zhelpers.hpp"
#include <thread>
#include <mutex>
#include <json.hpp>

class VariantClient
{
  std::thread thread_;
  std::mutex mutex;
  zmq::context_t context;
  zmq::socket_t request_socket;
  zmq::socket_t reply_socket;
  std::string variant_;
  unsigned id_;
  bool connection_ok;

  void main() {
    while(1) {
      zmq::message_t request;
      if (!request_socket.recv(&request)) {
        return;
      }
      zmq::message_t ack;
      if(request_socket.send(ack))
        connection_ok = true;

      std::string rcv_msg(static_cast<const char*>(request.data()), 
        request.size());
      
      id_ = [](std::string result_string) -> unsigned {
        nlohmann::json json = nlohmann::json::parse(std::move(result_string));
        unsigned msg_id = json["id"];
        std::string msg_type  = json["msg-type"];
        return msg_id;
      }(std::move(rcv_msg));
    }
  }

  public:
    VariantClient() :
        context(1),
        request_socket(context, ZMQ_REP),
        reply_socket(context, ZMQ_PUB),
        variant_(""),
        connection_ok(false) {
      request_socket.bind("tcp://*:8274");
      reply_socket.bind("tcp://*:8275");
      thread_ = std::thread(&VariantClient::main, this);
    }

    ~VariantClient() {
      thread_.join();
      request_socket.disconnect("tcp://*:8274");
      reply_socket.disconnect("tcp://*:8275");
    }

    void sendVariant(std::string value) {
      const std::lock_guard<std::mutex> lock(mutex);
      variant_ = std::move(value);
      if(connection_ok) {
        auto reply_msg = [&]() -> std::string {
          nlohmann::json json;
          json["msg-type"] = "variant";
          json["id"] = id_;
          auto& result = json["result"] = nlohmann::json({});
          result["value"] = variant_;
          return json.dump(1);
        }();
        zmq::message_t reply(reply_msg.size());
        memcpy(reply.data(), reply_msg.c_str(), reply_msg.size());
        reply_socket.send(reply);
      }
    }
};