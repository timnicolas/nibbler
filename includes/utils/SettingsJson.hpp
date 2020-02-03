#pragma once

#include <map>
#include <string>

template<class T>
class JsonObj {
	public:
		JsonObj() : _hasMin(false), _hasMax(false) {}
		virtual ~JsonObj() {}

		T const &		get() const { return _value; }
		T &				get() { return _value; }
		JsonObj<T> &	setValue(T value) {
			if (_hasMin && value < _min) {
				logWarn("unable to set arg (" << value << " < " << _min << ")");
				return *this;
			}
			if (_hasMax && value > _max) {
				logWarn("unable to set arg (" << value << " > " << _max << ")");
				return *this;
			}
			_value = value;
			return *this;
		}
		JsonObj<T> &	setMin(T value) { _hasMin = true; _min = value; return *this; }
		JsonObj<T> &	setMax(T value) { _hasMax = true; _max = value; return *this; }

	protected:
		bool	_hasMin;
		T		_min;
		bool	_hasMax;
		T		_max;
		T		_value;
};

class SettingsJson {
	public:
		SettingsJson();
		SettingsJson(SettingsJson const &src);
		virtual ~SettingsJson();

		SettingsJson &operator=(SettingsJson const &rhs);

		// int
		JsonObj<int64_t> &	addi(std::string name);
		JsonObj<int64_t> &	updatei(std::string name);
		int64_t				geti(std::string name) const;
		// double
		JsonObj<double> &	addf(std::string name);
		JsonObj<double> &	updatef(std::string name);
		double				getf(std::string name) const;
		// bool
		JsonObj<bool> &	addb(std::string name);
		JsonObj<bool> &	updateb(std::string name);
		bool			getb(std::string name) const;
		// string
		JsonObj<std::string> &	adds(std::string name);
		JsonObj<std::string> &	updates(std::string name);
		std::string const &		gets(std::string name) const;
		std::string &			gets(std::string name);
		// json
		JsonObj<SettingsJson> &	addj(std::string name);
		JsonObj<SettingsJson> &	updatej(std::string name);
		SettingsJson const &	getj(std::string name) const;
		SettingsJson &			getj(std::string name);

		class SettingsException : public std::runtime_error {
			public:
				SettingsException();
				explicit SettingsException(const char* what_arg);
				explicit SettingsException(const std::string what_arg);
		};

	private:
		std::map<std::string, JsonObj<int64_t>>			_intMap;  // i
		std::map<std::string, JsonObj<double>>			_doubleMap;  // f
		std::map<std::string, JsonObj<bool>>			_boolMap;  // b
		std::map<std::string, JsonObj<std::string>>		_stringMap;  // s
		std::map<std::string, JsonObj<SettingsJson>>	_jsonMap;  // j
};