#include <vector>
#include <iostream>
#include <zmq.hpp>
#include <string>
#include <algorithm>
#include "../protobuf/Services.pb.h"

#include "register.hpp"

namespace DDClient {
	void unregister(std::string const &name);
	class RegistrationManager {
		std::vector<std::string> registered_names;
		int registered_count;

		RegistrationManager() {
		}

		public:

		void unregister_all() {
			for (auto &name : registered_names) {
				unregister(name);
			}
			registered_names.clear();
		}

		void add(std::string const &name) {
			registered_names.push_back(name);
			registered_count++;
		}

		void remove(std::string const &name) {
			auto result = find(registered_names.begin(), registered_names.end(), name);
			if (result != registered_names.end()) {
				registered_names.erase(result);
				registered_count--;
			}
		}

		~RegistrationManager() {
			try {
				unregister_all();
			}
			catch (std::string &error) {
				;
			}
		}

		static RegistrationManager &instance() {
			static RegistrationManager instance;
			return instance;
		}
	};

	void register_service(std::string const &name, int port, std::map<std::string, std::string> const &txt) {
		directoryd::ServiceRequest request;
		request.set_type(directoryd::REGISTER);
		auto *r = request.mutable_register_();
		auto l = r->add_location();
		l->set_port(port);
		l->set_type("_hotdec._tcp");
		r->set_name(name);
		for (auto &t : txt) {
			auto txtfield = r->add_txt();
			txtfield->set_key(t.first);
			txtfield->set_value(t.second);
		}
		zmq::message_t message(request.ByteSize());
		request.SerializeToArray(message.data(), message.size());

		DDClient::instance().register_socket().send(message);
		message.rebuild();
		DDClient::instance().register_socket().recv(&message);
		directoryd::ServiceReply reply;
		reply.ParseFromArray(message.data(), message.size());

		if (reply.type() != directoryd::REGISTER) {
			throw RegistrationError("Got back incorrect message type when trying to register.");
		}
		if (reply.success() != true) {
			throw RegistrationError(reply.result());
		}

		RegistrationManager::instance().add(name);
	}

	void unregister(std::string const &name) {
		directoryd::ServiceRequest request;
		request.set_type(directoryd::UNREGISTER);
		auto *r = request.mutable_unregister();
		r->set_name(name);
		zmq::message_t message(request.ByteSize());
		request.SerializeToArray(message.data(), message.size());

		DDClient::instance().register_socket().send(message);
		message.rebuild();
		DDClient::instance().register_socket().recv(&message);
		directoryd::ServiceReply reply;
		reply.ParseFromArray(message.data(), message.size());

		if (reply.type() != directoryd::UNREGISTER) {
			throw RegistrationError("Got back incorrect message type when trying to unregister.");
		}
		if (reply.success() != true) {
			throw RegistrationError(reply.result());
		}
	}

	void unregister_service(std::string const &name) {
		unregister(name);
		RegistrationManager::instance().remove(name);
	}

	void heartbeat() {
		directoryd::ServiceRequest request;
		request.set_type(directoryd::HEARTBEAT);
		zmq::message_t message(request.ByteSize());
		request.SerializeToArray(message.data(), message.size());

		DDClient::instance().register_socket().send(message);
		message.rebuild();
		DDClient::instance().register_socket().recv(&message);
		directoryd::ServiceReply reply;
		reply.ParseFromArray(message.data(), message.size());

		if (reply.type() != directoryd::HEARTBEAT) {
			throw RegistrationError("Got back incorrect message type when trying to send heartbeat.");
		}
		if (reply.success() != true) {
			throw RegistrationError(reply.result());
		}
	}
}

