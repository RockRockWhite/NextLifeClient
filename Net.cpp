#include "Net.h"
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

typedef websocketpp::client<websocketpp::config::asio_client> Client;
typedef websocketpp::config::asio_client::message_type::ptr msg_ptr;

Client client_;
Client::connection_ptr connection_;
void OnMessage(Client *clinet,websocketpp::connection_hdl hdl, msg_ptr msg)
{

}

Net::Net()
{
	client_.init_asio();
	//client_.start_perpetual();
	client_.set_message_handler(bind(&OnMessage, &client_, placeholders::_1, placeholders::_2));
}
Net::~Net()
{
	//free
	error_code ec;
	client_.close(connection_->get_handle(), websocketpp::close::status::going_away, "", ec);
	RunThread->join();
}
error_code Net::send(string message)
{
	error_code ec;
	client_.send(connection_->get_handle(), message, websocketpp::frame::opcode::text, ec);
	return ec;
}
error_code Net::connect(string url)
{
	error_code ec;
	connection_=client_.get_connection(url, ec);

	//If error ,return
	if (ec)
		return ec;

	client_.connect(connection_);
	RunThread =make_shared<thread>(&Client::run, &client_);
	return ec;
}