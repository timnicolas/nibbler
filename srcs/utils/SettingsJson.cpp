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
	if (this != &rhs) {
		intMap = rhs.intMap;
		uintMap = rhs.uintMap;
		doubleMap = rhs.doubleMap;
		boolMap = rhs.boolMap;
		stringMap = rhs.stringMap;
		jsonMap = rhs.jsonMap;
	}
	return *this;
}

// int
JsonObj<int64_t> &	SettingsJson::addi(std::string name) {
	if (intMap.find(name) != intMap.end()) {
		logWarn("cannot add setting " << name << ": setting already exist");
		return intMap[name];
	}
	intMap.insert(std::pair<std::string, JsonObj<int64_t>>(name, JsonObj<int64_t>()));
	return intMap[name];
}
JsonObj<int64_t> &	SettingsJson::updatei(std::string name) {
	if (intMap.find(name) != intMap.end()) {
		return intMap[name];
	}
	throw SettingsException("undefined setting " + name);
}
int64_t SettingsJson::i(std::string name) const { return geti(name); }
int64_t SettingsJson::geti(std::string name) const {
	if (intMap.find(name) != intMap.end())
		return intMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}
// uint
JsonObj<uint64_t> &	SettingsJson::addu(std::string name) {
	if (uintMap.find(name) != uintMap.end()) {
		logWarn("cannot add setting " << name << ": setting already exist");
		return uintMap[name];
	}
	uintMap.insert(std::pair<std::string, JsonObj<uint64_t>>(name, JsonObj<uint64_t>()));
	return uintMap[name];
}
JsonObj<uint64_t> &	SettingsJson::updateu(std::string name) {
	if (uintMap.find(name) != uintMap.end()) {
		return uintMap[name];
	}
	throw SettingsException("undefined setting " + name);
}
uint64_t SettingsJson::u(std::string name) const { return getu(name); }
uint64_t SettingsJson::getu(std::string name) const {
	if (uintMap.find(name) != uintMap.end())
		return uintMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}
// double
JsonObj<double> &	SettingsJson::addf(std::string name) {
	if (doubleMap.find(name) != doubleMap.end()) {
		logWarn("cannot add setting " << name << ": setting already exist");
		return doubleMap[name];
	}
	doubleMap.insert(std::pair<std::string, JsonObj<double>>(name, JsonObj<double>()));
	return doubleMap[name];
}
JsonObj<double> &	SettingsJson::updatef(std::string name) {
	if (doubleMap.find(name) != doubleMap.end()) {
		return doubleMap[name];
	}
	throw SettingsException("undefined setting " + name);
}
double SettingsJson::f(std::string name) const { return getf(name); }
double SettingsJson::getf(std::string name) const {
	if (doubleMap.find(name) != doubleMap.end())
		return doubleMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}
// bool
JsonObj<bool> &	SettingsJson::addb(std::string name) {
	if (boolMap.find(name) != boolMap.end()) {
		logWarn("cannot add setting " << name << ": setting already exist");
		return boolMap[name];
	}
	boolMap.insert(std::pair<std::string, JsonObj<bool>>(name, JsonObj<bool>()));
	return boolMap[name];
}
JsonObj<bool> &	SettingsJson::updateb(std::string name) {
	if (boolMap.find(name) != boolMap.end()) {
		return boolMap[name];
	}
	throw SettingsException("undefined setting " + name);
}
bool SettingsJson::b(std::string name) const { return getb(name); }
bool SettingsJson::getb(std::string name) const {
	if (boolMap.find(name) != boolMap.end())
		return boolMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}
// string
JsonObj<std::string> &	SettingsJson::adds(std::string name) {
	if (stringMap.find(name) != stringMap.end()) {
		logWarn("cannot add setting " << name << ": setting already exist");
		return stringMap[name];
	}
	stringMap.insert(std::pair<std::string, JsonObj<std::string>>(name, JsonObj<std::string>()));
	return stringMap[name];
}
JsonObj<std::string> &	SettingsJson::updates(std::string name) {
	if (stringMap.find(name) != stringMap.end()) {
		return stringMap[name];
	}
	throw SettingsException("undefined setting " + name);
}
std::string const & SettingsJson::s(std::string name) const { return gets(name); }
std::string const & SettingsJson::gets(std::string name) const {
	if (stringMap.find(name) != stringMap.end())
		return stringMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}
std::string & SettingsJson::s(std::string name) { return gets(name); }
std::string & SettingsJson::gets(std::string name) {
	if (stringMap.find(name) != stringMap.end())
		return stringMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}
// json
SettingsJson &	SettingsJson::addj(std::string name) {
	if (jsonMap.find(name) != jsonMap.end()) {
		logWarn("cannot add setting " << name << ": setting already exist");
		return jsonMap[name].get();
	}
	jsonMap.insert(std::pair<std::string, JsonObj<SettingsJson>>(name, JsonObj<SettingsJson>()));
	return jsonMap[name].get();
}
SettingsJson &	SettingsJson::updatej(std::string name) {
	if (jsonMap.find(name) != jsonMap.end()) {
		return jsonMap[name].get();
	}
	throw SettingsException("undefined setting " + name);
}
SettingsJson const & SettingsJson::j(std::string name) const { return getj(name); }
SettingsJson const & SettingsJson::getj(std::string name) const {
	if (jsonMap.find(name) != jsonMap.end())
		return jsonMap.at(name).get();
	throw SettingsException("undefined setting " + name);
}
SettingsJson & SettingsJson::j(std::string name) { return getj(name); }
SettingsJson & SettingsJson::getj(std::string name) {
	if (jsonMap.find(name) != jsonMap.end())
		return jsonMap.at(name).get();
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
		jsonString<std::map<std::string, JsonObj<std::string>>>(out, it->second.get().stringMap, nbTab + 1, "\"", "\"");
		jsonString<std::map<std::string, JsonObj<uint64_t>>>(out, it->second.get().uintMap, nbTab + 1);
		jsonString<std::map<std::string, JsonObj<int64_t>>>(out, it->second.get().intMap, nbTab + 1);
		jsonString<std::map<std::string, JsonObj<double>>>(out, it->second.get().doubleMap, nbTab + 1);
		jsonString<std::map<std::string, JsonObj<bool>>>(out, it->second.get().boolMap, nbTab + 1);
		jsonStringRecursiv(out, it->second.get().jsonMap, nbTab + 1);
		for (int i = 0; i < nbTab; i++)
			out << "\t";
		out << "}," << std::endl;
	}
}

std::ostream & operator<<(std::ostream & out, const SettingsJson & s) {
	out << "{" << std::endl;
	int nbTab = 0;
	jsonString<std::map<std::string, JsonObj<std::string>>>(out, s.stringMap, nbTab + 1, "\"", "\"");
	jsonString<std::map<std::string, JsonObj<uint64_t>>>(out, s.uintMap, nbTab + 1);
	jsonString<std::map<std::string, JsonObj<int64_t>>>(out, s.intMap, nbTab + 1);
	jsonString<std::map<std::string, JsonObj<double>>>(out, s.doubleMap, nbTab + 1);
	jsonString<std::map<std::string, JsonObj<bool>>>(out, s.boolMap, nbTab + 1);
	jsonStringRecursiv(out, s.jsonMap, 1);
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
