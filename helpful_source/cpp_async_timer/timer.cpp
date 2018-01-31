#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

void print(const boost::system::error_code& /*e*/)
{
	std::cout << "Hello, world!\n";	
}

int main()
{
	for(int i=0; i<3; i++){
		boost::asio::io_service io;

		boost::asio::deadline_timer t(io, boost::posix_time::seconds(3));
		t.async_wait(&print);

		io.run();
	}
	return 0;
}