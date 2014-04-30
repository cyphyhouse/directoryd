#include <zmq.h>
#include "../protobuf/Services.pb.h"
#include "ddclient.hpp"

namespace DDClient {
    DDClient::DDClient () {
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	context_ = zmq_ctx_new ();
	query_socket_ = zmq_socket (context_, ZMQ_REQ);
	register_socket_  = zmq_socket (context_, ZMQ_REQ);

	assert(zmq_connect (query_socket_,"ipc:///tmp/directoryd") == 0);
	assert(zmq_connect (register_socket_,"ipc:///tmp/directoryd") == 0);
    }

    DDClient::~DDClient () {
	google::protobuf::ShutdownProtobufLibrary();
    }

    DDClient &DDClient::instance() {
	static DDClient instance;
	return instance;
    }
};
