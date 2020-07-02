#include "zhelpers.hpp"
#include "positions.hpp"
#include "change_velocity.hpp"
#include <thread>
#include <mutex>
#include "variant_client.hpp"
#include "query_client.hpp"

using namespace zmqserver;

int main(int argc, char *argv[])
{
  VariantClient variant_client;
  QueryClient query_client;

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
          query_client.main();
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
        query_client.main();
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
