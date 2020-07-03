/* Copyright (C) 2020 Antonio Bandera -  All Rights Reserved
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

#include "zhelpers.hpp"
#include "positions.hpp"
#include "change_velocity.hpp"
#include <thread>
#include <mutex>
#include "variant_client.hpp"
#include "query_client.hpp"
#include "mironDDS_listener.hpp"

using namespace zmqserver;

int main(int argc, char *argv[])
{
  VariantClient variant_client;
  QueryClient query_client;
  MironDDSListener mironListener(&query_client);

  while(1) {
    int type;
    int subtype;
    double value;

    // Menu for choosing a query
    std::cout << "********** Main Menu **********\n";
    std::cout << "(1): change_parameter\n";
    std::cout << "(2): get_all_positions\n";
    std::cout << "(3): abort_current_skill\n";
    std::cout << "(4): variant\n>> ";
    std::cin >> type;

    // Prepare json according to your query
    switch (type)
    {
      case 1: // Different options for change_parameter
      {
        std::cout << "*** Change_parameter Menu ***\n";
        std::cout << "(1): speed change\n>> ";
        std::cin >> subtype;
        switch (subtype)
        {
        case 1:
        {
          std::cout << "***** Speed_change value *****\n";
          std::cout << "speed value\n>> ";
          std::cin >> value;
          Velocity vel(value, 0);
          ChangeVelocity vel_msg(query_client.getID(), vel);
          query_client.setMsg(std::move(vel_msg.dump()));
          query_client.send();
          break;
        }
        default:
          std::cout << "ERROR! You have selected an invalid choice.\n";
          break;
        }
        break;
      }
      case 2:
      {
        Positions pos_msg(query_client.getID());
        query_client.setMsg(std::move(pos_msg.dump()));
        query_client.send();
        break;
      }
      case 3:
      {
        break;
      }
      case 4:
      {
        std::cout << "New Variant Value >> ";
        std::string variant;
        std::cin >> variant;
        variant_client.sendVariant(variant);
        break;
      }
    }
  }
}
