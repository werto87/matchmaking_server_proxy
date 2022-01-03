#include "matchmakingStateMachine.hxx"
#include "matchmaking_proxy/userMatchmakingSerialization.hxx" // for userMa...
#include <confu_json/to_object.hxx>                           // for to_object
#include <confu_json/util.hxx>                                // for type_name
#include <functional>                                         // for __base
#include <iostream>                                           // for string
#include <sstream>                                            // for basic_...
#include <string>                                             // for operat...
#include <utility>                                            // for pair
#include <vector>                                             // for vector
template <class T> class MyWebsocket;

void
MatchmakingStateMachine::init (std::shared_ptr<MyWebsocket<SSLWebsocket>> myWebsocket, boost::asio::io_context &executor, std::list<MatchmakingStateMachine>::iterator matchmaking, std::list<MatchmakingStateMachine> &matchmakings)
{
  co_spawn (
      executor,
      [myWebsocket, matchmaking] () mutable {
        using namespace boost::asio::experimental::awaitable_operators;
        return myWebsocket->readLoop ([matchmaking] (std::string msg) {
          std::vector<std::string> splitMesssage{};
          boost::algorithm::split (splitMesssage, msg, boost::is_any_of ("|"));
          if (splitMesssage.size () == 2)
            {
              auto const &typeToSearch = splitMesssage.at (0);
              auto const &objectAsString = splitMesssage.at (1);
              bool typeFound = false;
              auto objectAsStringStream = std::stringstream{};
              objectAsStringStream << objectAsString;
              boost::hana::for_each (user_matchmaking::userMatchmaking, [&] (const auto &x) {
                if (typeToSearch == confu_json::type_name<typename std::decay<decltype (x)>::type> ())
                  {
                    typeFound = true;
                    boost::json::error_code ec{};
                    matchmaking->matchmakingStateMachine.process_event (confu_json::to_object<std::decay_t<decltype (x)>> (confu_json::read_json (objectAsStringStream, ec)));
                    if (ec) std::cout << "ec.message () " << ec.message () << std::endl;
                    return;
                  }
              });
              if (not typeFound) std::cout << "could not find a match for typeToSearch '" << typeToSearch << "'" << std::endl;
            }
        }) || myWebsocket->writeLoop ();
      },
      [matchmaking, &matachmakings = matchmakings] (auto, auto) { matachmakings.erase (matchmaking); });
}