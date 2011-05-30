#include <gtest/gtest.h>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include "Rpc/RpcChannel.h"

namespace Egametang {

static int port = 10001;

class RpcServerTest
{
public:
	boost::asio::io_service io_service_;
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::ip::tcp::socket socket_;

	int size;

public:
	RpcServerTest(boost::asio::io_service& service, int port):
		io_service_(service), size(0)
	{
		boost::asio::ip::address address;
		address.from_string("localhost");
		boost::asio::ip::tcp::endpoint endpoint(address, port);
		boost::asio::ip::tcp::acceptor acceptor;
		acceptor.open(endpoint.protocol());
		acceptor.set_option(
		boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor.bind(endpoint);
		acceptor.listen();
		acceptor.async_accept(socket_);
	}
	~RpcServerTest();

	void RecvMessageSize()
	{
	}
};

class RpcChannelTest: public testing::Test
{
private:
	int port;
public:
	RpcChannelTest()
	{
	}

	void SetUp()
	{
		port = 10001;
	}

	void TearDown()
	{
	}
};


TEST_F(RpcChannelTest, CallMethod)
{
	RpcServerTest server(io_service_, port);
	ASSERT_EQ(0, server.size);

	RpcChannel channel(io_service_, "localhost", port);
	channel.CallMethod(NULL, NULL, request, response_, done_);

	ASSERT_EQ(request.ByteSize(), server.size);
}

} // namespace Egametang