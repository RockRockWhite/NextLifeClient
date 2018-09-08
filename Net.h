#pragma once
//Used WebSocketPP(https://github.com/zaphoyd/websocketpp)

#include <functional>//placeholder and bind()
#include <memory>
#include <thread>
#include <system_error>
using namespace std;

class Net
{
public:
	Net();
	~Net();
	error_code  connect(string url);

private:
	shared_ptr<thread> RunThread;
};
