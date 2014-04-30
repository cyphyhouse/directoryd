#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>

#include <zmq.hpp>

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
		zmq::message_t message(request.ByteSize());
		request.SerializeToArray(message.data(), message.size());

		DDClient::instance().query_socket().send(message);
		message.rebuild();
		DDClient::instance().query_socket().recv(&message);
		directoryd::ServiceReply reply;
		reply.ParseFromArray(message.data(), message.size());

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
