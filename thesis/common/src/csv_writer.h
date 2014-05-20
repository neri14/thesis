#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include <map>
#include <set>
#include <string>
#include <fstream>

namespace common {

class csv_writer
{
public:
	csv_writer(const std::string& filename_);
	virtual ~csv_writer();

	void add_key(const std::string& key);
	void add_value(int tick, const std::string& key, int value);
	void add_value(int tick, const std::string& key, const std::string& value);

	int key_count();

private:
	void write(const std::map<std::string, std::string>& record);

	std::string filename;
	std::ofstream file;

	std::set<std::string> keys;
	std::map<int, std::set<std::string> > pending_record;
	std::map<int, std::map<std::string, std::string> > records;
};

csv_writer& get_csv_writer();
csv_writer& get_csv_writer_opt();

} // namespace common

#endif /*CSV_WRITER_H*/