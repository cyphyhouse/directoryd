#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>

#include <zmq.h>

#include "../protobuf/Services.pb.h"
#include "ddclient.hpp"
#include "query.hpp"

namespace DDClient {
	std::vector<Service> find(std::map<std::string, std::string> const &txt, std::string const &name) {
		directoryd::ServiceRequest request;
		request.set_type(directoryd::FIND);
		directoryd::ServiceRequest::Find *f = request.mutable_find();
		f->set_type("_hotdec._tcp");
		if (name.empty() == false) {
			f->set_name(name);
		}
		for (auto &t : txt) {
			directoryd::TxtField *txtfield = f->add_txt();
			txtfield->set_key(t.first);
			txtfield->set_value(t.second);
		}
		zframe_t *sf = zframe_new(NULL, request.ByteSize());
		assert (sf != NULL);
		//zmq_msg_t message;
		//assert(zmq_msg_init_size (&message, request.ByteSize()) == 0);
		//		zmq::message_t message(request.ByteSize());

		request.SerializeToArray(zframe_data(sf),zframe_size(sf));

		int retval = zframe_send(&sf, DDClient::instance().query_socket(), 0);
		assert(retval == 0);

		//DDClient::instance().query_socket().send(message);
		//message.rebuild();

		zframe_t *rf = zframe_recv (DDClient::instance().query_socket());

		//DDClient::instance().query_socket().recv(&message);


		directoryd::ServiceReply reply;
		reply.ParseFromArray(zframe_data(rf),zframe_size(rf));
		//message.data(), message.size());
		zframe_destroy(&rf);

		std::vector<Service> services;
		if (reply.type() != directoryd::FIND) {
			throw QueryError("Got back incorrect message type when trying to query.");
		}
		if (reply.success() != true) {
			throw QueryError(reply.result());
		}
		for (int i = 0; i < reply.findresult_size(); ++i) {
			Service s;
			auto location = reply.findresult(i).location();
			s.hostname = location.host();
			s.port = location.port();
			for (int j = 0; j < reply.findresult(i).txt_size(); ++j) {
				auto t = reply.findresult(i).txt(j);
				s.txt[t.key()] = t.value();
			}
			services.push_back(s);
		}

		return services;
	}
}
