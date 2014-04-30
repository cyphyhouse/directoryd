#if !defined(_DDCLIENT_HPP)
#define _DDCLIENT_HPP
#include <zmq.h>

namespace DDClient {
class DDClient {
	zmq::context_t context_;
	zmq::socket_t query_socket_;
	zmq::socket_t register_socket_;
	DDClient ();
	~DDClient ();
public:

	zmq::socket_t &query_socket() {
		return query_socket_;
	}

	zmq::socket_t &register_socket() {
		return register_socket_;
	}

	static DDClient &instance();
};

}
#endif
