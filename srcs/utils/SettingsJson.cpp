#include "SettingsJson.hpp"

SettingsJson::SettingsJson() {
}

SettingsJson::SettingsJson(SettingsJson const &src) {
	*this = src;
}

SettingsJson::~SettingsJson() {
	deleteMap<std::string>(stringMap);
	deleteMap<int64_t>(intMap);
	deleteMap<uint64_t>(uintMap);
	deleteMap<double>(doubleMap);
	deleteMap<bool>(boolMap);
	deleteMap<SettingsJson>(jsonMap);
}

SettingsJson &SettingsJson::operator=(SettingsJson const &rhs) {
	if (this != &rhs) {
		logDebug("WARNING -> SettingsJson object copied");
		stringMap = copyMap<std::string>(rhs.stringMap);
		intMap = copyMap<int64_t>(rhs.intMap);
		uintMap = copyMap<uint64_t>(rhs.uintMap);
		doubleMap = copyMap<double>(rhs.doubleMap);
		boolMap = copyMap<bool>(rhs.boolMap);
		jsonMap = copyMap<SettingsJson>(rhs.jsonMap);
	}
	return *this;
}

bool SettingsJson::loadJson(nlohmann::json const & json, SettingsJson & jsonObjTmp) {
	bool ret = true;
	for (auto it = json.begin(); it != json.end(); ++it) {
		if (it->is_object()) {
			auto newJsonObj = jsonObjTmp.jsonMap.find(it.key());
			if (newJsonObj != jsonObjTmp.jsonMap.end()) {
				ret &= loadJson(*it, newJsonObj->second->get());
			}
			else {
				logWarn("invalid setting: " << it.key());
				ret = false;
			}
		}
		else {
			if (it->is_number_integer() && jsonObjTmp.intMap.find(it.key()) != jsonObjTmp.intMap.end()) {
				if (jsonObjTmp.update<int64_t>(it.key()).isDisabledInFile()) {
					logWarn("you can't set " << it.key() << " in setting file");
					ret = false;
				}
				else {
					ret &= jsonObjTmp.update<int64_t>(it.key()).checkValue(it->get<int64_t>());
					jsonObjTmp.update<int64_t>(it.key()).setValue(it->get<int64_t>());
				}
			}
			else if (it->is_number_unsigned() && jsonObjTmp.uintMap.find(it.key()) != jsonObjTmp.uintMap.end()) {
				if (jsonObjTmp.update<uint64_t>(it.key()).isDisabledInFile()) {
					logWarn("you can't set " << it.key() << " in setting file");
					ret = false;
				}
				else {
					ret &= jsonObjTmp.update<uint64_t>(it.key()).checkValue(it->get<uint64_t>());
					jsonObjTmp.update<uint64_t>(it.key()).setValue(it->get<uint64_t>());
				}
			}
			else if (it->is_number_float() && jsonObjTmp.doubleMap.find(it.key()) != jsonObjTmp.doubleMap.end()) {
				if (jsonObjTmp.update<double>(it.key()).isDisabledInFile()) {
					logWarn("you can't set " << it.key() << " in setting file");
					ret = false;
				}
				else {
					ret &= jsonObjTmp.update<double>(it.key()).checkValue(it->get<double>());
					jsonObjTmp.update<double>(it.key()).setValue(it->get<double>());
				}
			}
			else if (it->is_boolean() && jsonObjTmp.boolMap.find(it.key()) != jsonObjTmp.boolMap.end()) {
				if (jsonObjTmp.update<bool>(it.key()).isDisabledInFile()) {
					logWarn("you can't set " << it.key() << " in setting file");
					ret = false;
				}
				else {
					ret &= jsonObjTmp.update<bool>(it.key()).checkValue(it->get<bool>());
					jsonObjTmp.update<bool>(it.key()).setValue(it->get<bool>());
				}
			}
			else if (it->is_string() && jsonObjTmp.stringMap.find(it.key()) != jsonObjTmp.stringMap.end()) {
				if (jsonObjTmp.update<std::string>(it.key()).isDisabledInFile()) {
					logWarn("you can't set " << it.key() << " in setting file");
					ret = false;
				}
				else {
					ret &= jsonObjTmp.update<std::string>(it.key()).checkValue(it->get<std::string>());
					jsonObjTmp.update<std::string>(it.key()).setValue(it->get<std::string>());
				}
			}
			else {
				ret = false;
				logWarn("invalid setting type or name: " << it.key());
			}
		}
	}
	return ret;
}

bool SettingsJson::loadFile(std::string const &filename) {
	try {
		std::ifstream fileStream(filename, std::ifstream::in);

		nlohmann::json	data;
		if (fileStream.is_open()) {
			fileStream >> data;
		}
		else {
			throw SettingsException("invalid file format: " + filename);
		}
		return loadJson(data, *this);
	}
	catch (const nlohmann::json::parse_error& e) {
		throw SettingsException("invalid file format: " + filename);
	}
	catch (std::exception &e) {
		throw SettingsException("unable to open file: " + filename);
	}
	return true;
}

void SettingsJson::saveToFile(std::string const & filename) {
	std::ofstream settingsFile(filename);
	if (settingsFile.fail()) {
		throw SettingsException("unable to save settings file " + filename + ": " + strerror(errno));
	}
	settingsFile << toString();
	if (settingsFile.fail()) {
		throw SettingsException("unable to save settings file " + filename + ": " + strerror(errno));
	}
	settingsFile.close();
}

std::string SettingsJson::toString() const {
	std::ostringstream out;
	out << *this;
	return out.str();
}

// -- cout --------------------------------------------------------
template<class T>
int jsonString(std::ostream & out, T const & map, int nbTab, int nbElemRemain,
std::string const & before = "", std::string const & after = "") {
	for (auto it = map.begin(); it != map.end(); it++) {
		nbElemRemain--;
		for (int i = 0; i < nbTab; i++)
			out << "\t";
		out << '"' << it->first << "\": " << before << *(it->second) << after;
		if (nbElemRemain > 0)
			out << ",";
		out << std::endl;
	}
	return map.size();
}

int jsonStringRecursiv(std::ostream & out, std::map<std::string, JsonObj<SettingsJson> *> const & map, int nbTab) {
	int nbRem = map.size();
	for (auto it = map.begin(); it != map.end(); it++) {
		nbRem--;
		for (int i = 0; i < nbTab; i++)
			out << "\t";
		if (it->first == "")
			out << "{" << std::endl;
		else
			out << '"' <<  it->first << "\": {" << std::endl;
		int nbElem = it->second->get().stringMap.size()
					+ it->second->get().uintMap.size()
					+ it->second->get().intMap.size()
					+ it->second->get().doubleMap.size()
					+ it->second->get().boolMap.size()
					+ it->second->get().jsonMap.size();
		nbElem -= jsonString<std::map<std::string, JsonObj<std::string> *>>(out, it->second->get().stringMap, nbTab + 1,
																			nbElem, "\"", "\"");
		nbElem -= jsonString<std::map<std::string, JsonObj<uint64_t> *>>(out, it->second->get().uintMap, nbTab + 1, nbElem);
		nbElem -= jsonString<std::map<std::string, JsonObj<int64_t> *>>(out, it->second->get().intMap, nbTab + 1, nbElem);
		nbElem -= jsonString<std::map<std::string, JsonObj<double> *>>(out, it->second->get().doubleMap, nbTab + 1, nbElem);
		nbElem -= jsonString<std::map<std::string, JsonObj<bool> *>>(out, it->second->get().boolMap, nbTab + 1, nbElem);
		nbElem -= jsonStringRecursiv(out, it->second->get().jsonMap, nbTab + 1);
		for (int i = 0; i < nbTab; i++)
			out << "\t";
		out << "}";
		if (nbRem > 0)
			out << ",";
		out << std::endl;
	}
	return map.size();
}

std::ostream & operator<<(std::ostream & out, SettingsJson const & s) {
	out << "{" << std::endl;
	int nbElem = s.stringMap.size()
				+ s.uintMap.size()
				+ s.intMap.size()
				+ s.doubleMap.size()
				+ s.boolMap.size()
				+ s.jsonMap.size();
	nbElem -= jsonString<std::map<std::string, JsonObj<std::string> *>>(out, s.stringMap, 1, nbElem, "\"", "\"");
	nbElem -= jsonString<std::map<std::string, JsonObj<uint64_t> *>>(out, s.uintMap, 1, nbElem);
	nbElem -= jsonString<std::map<std::string, JsonObj<int64_t> *>>(out, s.intMap, 1, nbElem);
	nbElem -= jsonString<std::map<std::string, JsonObj<double> *>>(out, s.doubleMap, 1, nbElem);
	nbElem -= jsonString<std::map<std::string, JsonObj<bool> *>>(out, s.boolMap, 1, nbElem);
	nbElem -= jsonStringRecursiv(out, s.jsonMap, 1);
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
