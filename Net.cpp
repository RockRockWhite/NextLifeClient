#include "Net.h"
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

typedef websocketpp::client<websocketpp::config::asio_client> Client;

Client client_;
Client::connection_ptr connection_;

Net::Net()
{
	client_.init_asio();
	client_.start_perpetual();
}
Net::~Net()
{
}

void Net::connect(string url)
{
	error_code ec;
	connection_=client_.get_connection(url, ec);

}