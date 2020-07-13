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

    Authors:  Antonio Bandera, ajbandera@uma.es 
              Renan Freitas, renan028@gmail.com
    Maintainer: Renan Freitas, renan028@gmail.com
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
  auto variant_client = std::make_shared<VariantClient>();
  auto query_client = std::make_shared<QueryClient>();
  MironDDSListener mironListener(query_client, variant_client);

  while(1) {
    int type;
    int subtype;

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
          double vmin = 0;
          double vmax = 1000;
          std::cout << "***** Speed_change value *****\n";
          std::cout << "vmin\n>> ";
          std::cin >> vmin;
          std::cout << "vmax\n>> ";
          std::cin >> vmax;
          Velocity vel(vmin, vmax);
          ChangeVelocity vel_msg(query_client->getID(), vel);
          query_client->setMsg(std::move(vel_msg.dump()));
          query_client->send();
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
        Positions pos_msg(query_client->getID());
        query_client->setMsg(std::move(pos_msg.dump()));
        query_client->send();
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
        variant_client->sendVariant(variant);
        break;
      }
    }
  }
}
