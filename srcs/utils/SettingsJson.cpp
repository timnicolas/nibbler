#include "SettingsJson.hpp"
#include "Logging.hpp"

SettingsJson::SettingsJson() {
}

SettingsJson::SettingsJson(SettingsJson const &src) {
	*this = src;
}

SettingsJson::~SettingsJson() {
}

SettingsJson &SettingsJson::operator=(SettingsJson const &rhs) {
	(void)rhs;
	// if (this != &rhs) {}
	return *this;
}

// int
JsonObj<int64_t> &	SettingsJson::addi(std::string name) {
	if (_intMap.find(name) != _intMap.end()) {
		logWarn("cannot add setting " << name << ": setting already exist");
		return _intMap[name];
	}
	_intMap.insert(std::pair<std::string, JsonObj<int64_t>>(name, JsonObj<int64_t>()));
	return _intMap[name];
}
JsonObj<int64_t> &	SettingsJson::updatei(std::string name) {
	if (_intMap.find(name) != _intMap.end()) {
		return _intMap[name];
	}
	throw SettingsException("undefined setting " + name);
}
int64_t SettingsJson::i(std::string name) const { return geti(name); }
int64_t SettingsJson::geti(std::string name) const {
	if (_intMap.find(name) != _intMap.end())
		return _intMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}
// uint
JsonObj<uint64_t> &	SettingsJson::addu(std::string name) {
	if (_uintMap.find(name) != _uintMap.end()) {
		logWarn("cannot add setting " << name << ": setting already exist");
		return _uintMap[name];
	}
	_uintMap.insert(std::pair<std::string, JsonObj<uint64_t>>(name, JsonObj<uint64_t>()));
	return _uintMap[name];
}
JsonObj<uint64_t> &	SettingsJson::updateu(std::string name) {
	if (_uintMap.find(name) != _uintMap.end()) {
		return _uintMap[name];
	}
	throw SettingsException("undefined setting " + name);
}
uint64_t SettingsJson::u(std::string name) const { return getu(name); }
uint64_t SettingsJson::getu(std::string name) const {
	if (_uintMap.find(name) != _uintMap.end())
		return _uintMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}
// double
JsonObj<double> &	SettingsJson::addf(std::string name) {
	if (_doubleMap.find(name) != _doubleMap.end()) {
		logWarn("cannot add setting " << name << ": setting already exist");
		return _doubleMap[name];
	}
	_doubleMap.insert(std::pair<std::string, JsonObj<double>>(name, JsonObj<double>()));
	return _doubleMap[name];
}
JsonObj<double> &	SettingsJson::updatef(std::string name) {
	if (_doubleMap.find(name) != _doubleMap.end()) {
		return _doubleMap[name];
	}
	throw SettingsException("undefined setting " + name);
}
double SettingsJson::f(std::string name) const { return getf(name); }
double SettingsJson::getf(std::string name) const {
	if (_doubleMap.find(name) != _doubleMap.end())
		return _doubleMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}
// bool
JsonObj<bool> &	SettingsJson::addb(std::string name) {
	if (_boolMap.find(name) != _boolMap.end()) {
		logWarn("cannot add setting " << name << ": setting already exist");
		return _boolMap[name];
	}
	_boolMap.insert(std::pair<std::string, JsonObj<bool>>(name, JsonObj<bool>()));
	return _boolMap[name];
}
JsonObj<bool> &	SettingsJson::updateb(std::string name) {
	if (_boolMap.find(name) != _boolMap.end()) {
		return _boolMap[name];
	}
	throw SettingsException("undefined setting " + name);
}
bool SettingsJson::b(std::string name) const { return getb(name); }
bool SettingsJson::getb(std::string name) const {
	if (_boolMap.find(name) != _boolMap.end())
		return _boolMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}
// string
JsonObj<std::string> &	SettingsJson::adds(std::string name) {
	if (_stringMap.find(name) != _stringMap.end()) {
		logWarn("cannot add setting " << name << ": setting already exist");
		return _stringMap[name];
	}
	_stringMap.insert(std::pair<std::string, JsonObj<std::string>>(name, JsonObj<std::string>()));
	return _stringMap[name];
}
JsonObj<std::string> &	SettingsJson::updates(std::string name) {
	if (_stringMap.find(name) != _stringMap.end()) {
		return _stringMap[name];
	}
	throw SettingsException("undefined setting " + name);
}
std::string const & SettingsJson::s(std::string name) const { return gets(name); }
std::string const & SettingsJson::gets(std::string name) const {
	if (_stringMap.find(name) != _stringMap.end())
		return _stringMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}
std::string & SettingsJson::s(std::string name) { return gets(name); }
std::string & SettingsJson::gets(std::string name) {
	if (_stringMap.find(name) != _stringMap.end())
		return _stringMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}
// json
SettingsJson &	SettingsJson::addj(std::string name) {
	if (_jsonMap.find(name) != _jsonMap.end()) {
		logWarn("cannot add setting " << name << ": setting already exist");
		return _jsonMap[name].get();
	}
	_jsonMap.insert(std::pair<std::string, JsonObj<SettingsJson>>(name, JsonObj<SettingsJson>()));
	return _jsonMap[name].get();
}
SettingsJson &	SettingsJson::updatej(std::string name) {
	if (_jsonMap.find(name) != _jsonMap.end()) {
		return _jsonMap[name].get();
	}
	throw SettingsException("undefined setting " + name);
}
SettingsJson const & SettingsJson::j(std::string name) const { return getj(name); }
SettingsJson const & SettingsJson::getj(std::string name) const {
	if (_jsonMap.find(name) != _jsonMap.end())
		return _jsonMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}
SettingsJson & SettingsJson::j(std::string name) { return getj(name); }
SettingsJson & SettingsJson::getj(std::string name) {
	if (_jsonMap.find(name) != _jsonMap.end())
		return _jsonMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}

// -- cout --------------------------------------------------------
template<class T>
void jsonString(std::ostream & out, T const & map, int nbTab,
std::string const & before = "", std::string const & after = "") {
	for (auto it = map.begin(); it != map.end(); it++) {
		for (int i = 0; i < nbTab; i++)
			out << "\t";
		out << '"' << it->first << "\": " << before << it->second.get() << after << "," << std::endl;
	}
}

void jsonStringRecursiv(std::ostream & out, std::map<std::string, JsonObj<SettingsJson>> const & map, int nbTab) {
	for (auto it = map.begin(); it != map.end(); it++) {
		for (int i = 0; i < nbTab; i++)
			out << "\t";
		out << '"' <<  it->first << "\": {" << std::endl;
		jsonString<std::map<std::string, JsonObj<std::string>>>(out, it->second.get()._stringMap, nbTab + 1, "\"", "\"");
		jsonString<std::map<std::string, JsonObj<uint64_t>>>(out, it->second.get()._uintMap, nbTab + 1);
		jsonString<std::map<std::string, JsonObj<int64_t>>>(out, it->second.get()._intMap, nbTab + 1);
		jsonString<std::map<std::string, JsonObj<double>>>(out, it->second.get()._doubleMap, nbTab + 1);
		jsonString<std::map<std::string, JsonObj<bool>>>(out, it->second.get()._boolMap, nbTab + 1);
		jsonStringRecursiv(out, it->second.get()._jsonMap, nbTab + 1);
		for (int i = 0; i < nbTab; i++)
			out << "\t";
		out << "}," << std::endl;
	}
}

std::ostream & operator<<(std::ostream & out, const SettingsJson & s) {
	out << "{" << std::endl;
	int nbTab = 0;
	jsonString<std::map<std::string, JsonObj<std::string>>>(out, s._stringMap, nbTab + 1, "\"", "\"");
	jsonString<std::map<std::string, JsonObj<uint64_t>>>(out, s._uintMap, nbTab + 1);
	jsonString<std::map<std::string, JsonObj<int64_t>>>(out, s._intMap, nbTab + 1);
	jsonString<std::map<std::string, JsonObj<double>>>(out, s._doubleMap, nbTab + 1);
	jsonString<std::map<std::string, JsonObj<bool>>>(out, s._boolMap, nbTab + 1);
	jsonStringRecursiv(out, s._jsonMap, 1);
	for (int i = 0; i < nbTab; i++)
		out << "\t";
	out << "}" << std::endl;
	return out;
}

// -- Exceptions errors --------------------------------------------------------
SettingsJson::SettingsException::SettingsException()
: std::runtime_error("[SettingsException]") {}

SettingsJson::SettingsException::SettingsException(const char* what_arg)
: std::runtime_error(std::string(std::string("[SettingsException] ") + what_arg).c_str()) {}

SettingsJson::SettingsException::SettingsException(const std::string what_arg)
: std::runtime_error(std::string(std::string("[SettingsException] ") + what_arg).c_str()) {}
