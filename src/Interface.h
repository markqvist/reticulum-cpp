#pragma once

#include "Log.h"
#include "Bytes.h"
#include "Type.h"

#include <list>
#include <memory>
#include <stdint.h>

namespace RNS {

	class Interface {

	public:
	class AnnounceEntry {
	public:
		AnnounceEntry() {}
		AnnounceEntry(const Bytes& destination, double time, uint8_t hops, double emitted, const Bytes& raw) :
			_destination(destination),
			_time(time),
			_hops(hops),
			_emitted(emitted),
			_raw(raw) {}
	public:
		Bytes _destination;
		double _time = 0;
		uint8_t _hops = 0;
		double _emitted = 0;
		Bytes _raw;
	};

	protected:
		using HInterface = std::shared_ptr<Interface>;

	public:
		// Which interface modes a Transport Node
		// should actively discover paths for.
		static uint8_t DISCOVER_PATHS_FOR;

	public:
		Interface(Type::NoneConstructor none) {
			mem("Interface object NONE created, this: " + std::to_string((uintptr_t)this) + ", data: " + std::to_string((uintptr_t)_object.get()));
		}
		Interface(const Interface& interface) : _object(interface._object) {
			mem("Interface object copy created, this: " + std::to_string((uintptr_t)this) + ", data: " + std::to_string((uintptr_t)_object.get()));
		}
		Interface() : _object(new Object(*this)) {
			mem("Interface object created, this: " + std::to_string((uintptr_t)this) + ", data: " + std::to_string((uintptr_t)_object.get()));
		}
		Interface(const char* name) : _object(new Object(*this, name)), _creator(true) {
			mem("Interface object created, this: " + std::to_string((uintptr_t)this) + ", data: " + std::to_string((uintptr_t)_object.get()));
		}
		virtual ~Interface() {
			if (_creator) {
				// clear reference to parent in object foir safety
				assert(_object);
				_object->_parent = {Type::NONE};
			}
			mem("Interface object destroyed, this: " + std::to_string((uintptr_t)this) + ", data: " + std::to_string((uintptr_t)_object.get()));
		}

		inline Interface& operator = (const Interface& interface) {
			_object = interface._object;
			mem("Interface object copy created by assignment, this: " + std::to_string((uintptr_t)this) + ", data: " + std::to_string((uintptr_t)_object.get()));
			return *this;
		}
		inline operator bool() const {
			extreme("Interface object bool, this: " + std::to_string((uintptr_t)this) + ", data: " + std::to_string((uintptr_t)_object.get()));
			return _object.get() != nullptr;
		}
		inline bool operator < (const Interface& interface) const {
			extreme("Interface object <, this: " + std::to_string((uintptr_t)this) + ", data: " + std::to_string((uintptr_t)_object.get()));
			return _object.get() < interface._object.get();
		}
		inline bool operator > (const Interface& interface) const {
			extreme("Interface object <, this: " + std::to_string((uintptr_t)this) + ", data: " + std::to_string((uintptr_t)_object.get()));
			return _object.get() > interface._object.get();
		}
		inline bool operator == (const Interface& interface) const {
			extreme("Interface object ==, this: " + std::to_string((uintptr_t)this) + ", data: " + std::to_string((uintptr_t)_object.get()));
			return _object.get() == interface._object.get();
		}
		inline bool operator != (const Interface& interface) const {
			extreme("Interface object !=, this: " + std::to_string((uintptr_t)this) + ", data: " + std::to_string((uintptr_t)_object.get()));
			return _object.get() != interface._object.get();
		}

	public:
		const Bytes get_hash() const;
		void process_announce_queue();
		inline void detach() {}

		inline void add_announce(AnnounceEntry& entry) { assert(_object); _object->_announce_queue.push_back(entry); }

	protected:
		// CBA TODO these should NOT called internally, and should likely be protected (only to be overridden and called by Object)
	    virtual void on_incoming(const Bytes& data);
		virtual void on_outgoing(const Bytes& data);

	private:
	    //inline void process_incoming(const Bytes& data) { assert(_object); _object->process_incoming(data); }
		inline void process_outgoing(const Bytes& data) { assert(_object); _object->process_outgoing(data); }

		// getters/setters
	protected:
		inline void IN(bool IN) { assert(_object); _object->_IN = IN; }
		inline void OUT(bool OUT) { assert(_object); _object->_OUT = OUT; }
		inline void FWD(bool FWD) { assert(_object); _object->_FWD = FWD; }
		inline void RPT(bool RPT) { assert(_object); _object->_RPT = RPT; }
		inline void name(const char* name) { assert(_object); _object->_name = name; }
		inline void bitrate(uint32_t bitrate) { assert(_object); _object->_bitrate = bitrate; }
		inline void online(bool online) { assert(_object); _object->_online = online; }
		inline void announce_allowed_at(double announce_allowed_at) { assert(_object); _object->_announce_allowed_at = announce_allowed_at; }
	public:
		inline bool IN() const { assert(_object); return _object->_IN; }
		inline bool OUT() const { assert(_object); return _object->_OUT; }
		inline bool FWD() const { assert(_object); return _object->_FWD; }
		inline bool RPT() const { assert(_object); return _object->_RPT; }
		inline bool online() const { assert(_object); return _object->_online; }
		inline std::string name() const { assert(_object); return _object->_name; }
		inline const Bytes& ifac_identity() const { assert(_object); return _object->_ifac_identity; }
		inline Type::Interface::modes mode() const { assert(_object); return _object->_mode; }
		inline void mode(Type::Interface::modes mode) { assert(_object); _object->_mode = mode; }
		inline uint32_t bitrate() const { assert(_object); return _object->_bitrate; }
		inline double announce_allowed_at() const { assert(_object); return _object->_announce_allowed_at; }
		inline float announce_cap() const { assert(_object); return _object->_announce_cap; }
		inline std::list<AnnounceEntry>& announce_queue() const { assert(_object); return _object->_announce_queue; }
		inline bool is_connected_to_shared_instance() const { assert(_object); return _object->_is_connected_to_shared_instance; }
		inline bool is_local_shared_instance() const { assert(_object); return _object->_is_local_shared_instance; }
		inline HInterface parent_interface() const { assert(_object); return _object->_parent_interface; }

		virtual inline std::string toString() const { assert(_object); return "Interface[" + _object->_name + "]"; }

		inline std::string debugString() const {
			std::string dump;
			dump = "Interface object, this: " + std::to_string((uintptr_t)this) + ", data: " + std::to_string((uintptr_t)_object.get());
			return dump;
		}

	private:
		class Object {
		public:
			Object(Interface& parent) : _parent(parent) { mem("Interface::Data object created, this: " + std::to_string((uintptr_t)this)); }
			Object(Interface& parent, const char* name) : _parent(parent), _name(name) { mem("Interface::Data object created, this: " + std::to_string((uintptr_t)this)); }
			virtual ~Object() { mem("Interface::Data object destroyed, this: " + std::to_string((uintptr_t)this)); }
		private:
			//virtual inline void process_incoming(const Bytes& data) { if (_parent) { _parent.on_incoming(data); } }
			virtual inline void process_outgoing(const Bytes& data) { if (_parent) { _parent.on_outgoing(data); } }
		private:
			Interface& _parent;
			bool _IN  = false;
			bool _OUT = false;
			bool _FWD = false;
			bool _RPT = false;
			std::string _name;
			size_t _rxb = 0;
			size_t _txb = 0;
			bool _online = false;
			Bytes _ifac_identity;
			Type::Interface::modes _mode = Type::Interface::MODE_NONE;
			uint32_t _bitrate = 0;
			double _announce_allowed_at = 0;
			float _announce_cap = 0.0;
			std::list<AnnounceEntry> _announce_queue;
			bool _is_connected_to_shared_instance = false;
			bool _is_local_shared_instance = false;
			HInterface _parent_interface;
			//Transport& _owner;
		friend class Interface;
		};
		std::shared_ptr<Object> _object;
		bool _creator = false;

	friend class Transport;
	};

}

namespace ArduinoJson {
	inline bool convertToJson(const RNS::Interface& src, JsonVariant dst) {
		if (!src) {
			return dst.set(nullptr);
		}
		return dst.set(src.get_hash().toHex());
	}
	void convertFromJson(JsonVariantConst src, RNS::Interface& dst);
	inline bool canConvertFromJson(JsonVariantConst src, const RNS::Interface&) {
		return src.is<const char*>() && strlen(src.as<const char*>()) == 64;
	}
}
