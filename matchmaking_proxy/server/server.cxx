#include "server.hxx"
#include <boost/certify/extensions.hpp>
#include <boost/certify/https_verification.hpp> //INCLUDING THIS IN MORE THAN ONE TRANSLATION UNITS LEADS TO MULTIPLE DEFINITIONS
Server::Server (boost::asio::io_context &io_context, boost::asio::thread_pool &pool) : _io_context{ io_context }, _pool{ pool } {}

awaitable<void>
Server::listener (boost::asio::ip::tcp::endpoint const &endpoint, std::filesystem::path const &pathToSecrets)
{
  try
    {
      auto executor = co_await this_coro::executor;
      tcp_acceptor acceptor (executor, endpoint);
      net::ssl::context ctx (net::ssl::context::tls_server);
      ctx.set_verify_mode (ssl::context::verify_peer);
      ctx.set_default_verify_paths ();
      try
        {
          ctx.use_certificate_chain_file (pathToSecrets / "fullchain.pem");
        }
      catch (std::exception &e)
        {
          std::cout << "load fullchain: " << pathToSecrets / "fullchain.pem"
                    << " exception : " << e.what () << std::endl;
        }
      try
        {
          ctx.use_private_key_file (pathToSecrets / "privkey.pem", boost::asio::ssl::context::pem);
        }
      catch (std::exception &e)
        {
          std::cout << "load privkey: " << pathToSecrets / "privkey.pem"
                    << " exception : " << e.what () << std::endl;
        }
      try
        {
          ctx.use_tmp_dh_file (pathToSecrets / "dh2048.pem");
        }
      catch (std::exception &e)
        {
          std::cout << "load dh2048: " << pathToSecrets / "dh2048.pem"
                    << " exception : " << e.what () << std::endl;
        }
      boost::certify::enable_native_https_server_verification (ctx);
      ctx.set_options (SSL_SESS_CACHE_OFF | SSL_OP_NO_TICKET); //  disable ssl cache. It has a bad support in boost asio/beast and I do not know if it helps in performance in our usecase
      for (;;)
        {
          try
            {
              auto socket = co_await acceptor.async_accept ();
              auto connection = std::make_shared<SSLWebsocket> (SSLWebsocket{ std::move (socket), ctx });
              connection->set_option (websocket::stream_base::timeout::suggested (role_type::server));
              connection->set_option (websocket::stream_base::decorator ([] (websocket::response_type &res) { res.set (http::field::server, std::string (BOOST_BEAST_VERSION_STRING) + " websocket-server-async"); }));
              co_await connection->next_layer ().async_handshake (ssl::stream_base::server, use_awaitable);
              co_await connection->async_accept (use_awaitable);
              auto myWebsocket = std::make_shared<MyWebsocket<SSLWebsocket>> (MyWebsocket<SSLWebsocket>{ connection });
              matchmakings.emplace_back (Matchmaking{ _io_context, users, _pool, gameLobbies, [myWebsocket] (std::string message) { myWebsocket->sendMessage (message); } });
              std::list<MatchmakingStateMachine>::iterator matchmaking = std::next (matchmakings.end (), -1);
              matchmaking->init (myWebsocket, _io_context, matchmaking, matchmakings);
            }
          catch (std::exception &e)
            {
              std::cout << "Server::listener () connect  Exception : " << e.what () << std::endl;
            }
        }
    }
  catch (std::exception &e)
    {
      std::cout << "exception: " << e.what () << std::endl;
    }
}
