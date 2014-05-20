#include "csv_writer.h"

#include <boost/foreach.hpp>
#include <sstream>
#include <iostream>

namespace common {
namespace constant {
const std::string filename("data.csv");
const std::string filename2("data2.csv");
}

csv_writer::csv_writer(const std::string& filename_) :
	filename(filename_)
{}

csv_writer::~csv_writer()
{
	file.close();
}

void csv_writer::add_key(const std::string& key)
{
	keys.insert(key);
}

void csv_writer::add_value(int tick, const std::string& key, int value)
{
	std::ostringstream ss;
	ss << value;
	add_value(tick, key, ss.str());
}

void csv_writer::add_value(int tick, const std::string& key, const std::string& value)
{
	if (pending_record.find(tick) == pending_record.end()) {
		pending_record[tick] = keys;
		records[tick] = std::map<std::string, std::string>();
	}

	pending_record[tick].erase(key);
	records[tick].insert(std::make_pair(key, value));

	if (pending_record[tick].empty()) {
		write(records[tick]);
		pending_record.erase(tick);
		records.erase(tick);
	}
}

void csv_writer::write(const std::map<std::string, std::string>& record)
{
	if (!file.is_open()) {
		file.open(filename.c_str());

		BOOST_FOREACH(std::string key, keys) {
			file << key << ";";
		}
		file << "\n";
	file.flush();
	}

	BOOST_FOREACH(std::string key, keys) {
		file << record.find(key)->second << ";";
	}
	file << "\n";
	file.flush();
}

int csv_writer::key_count()
{
	return keys.size();
}

csv_writer& get_csv_writer()
{
	static csv_writer instance(constant::filename);
	return instance;
}

csv_writer& get_csv_writer_opt()
{
	static csv_writer instance(constant::filename2);
	if (!instance.key_count()) {
		instance.add_key("name");
		instance.add_key("stops");
		instance.add_key("time");
	}
	return instance;
}

} // namespace common