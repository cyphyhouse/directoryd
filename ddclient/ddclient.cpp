#include <zmq.h>
#include "../protobuf/Services.pb.h"
#include "ddclient.hpp"

namespace DDClient {
	DDClient::DDClient () : context_(1), query_socket_(context_, ZMQ_REQ), register_socket_(context_, ZMQ_REQ) {
		GOOGLE_PROTOBUF_VERIFY_VERSION;
		query_socket_.connect("ipc:///tmp/directoryd");
		register_socket_.connect("ipc:///tmp/directoryd");
	}

	DDClient::~DDClient () {
		google::protobuf::ShutdownProtobufLibrary();
	}

	DDClient &DDClient::instance() {
		static DDClient instance;
		return instance;
	}
};
