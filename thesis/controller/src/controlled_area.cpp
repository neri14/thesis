#include "controlled_area.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace controller {

controlled_area::controlled_area(const std::string& filename_) :
	logger("controlled_area"),
	filename(filename_)
{}

controlled_area::~controlled_area()
{}

bool controlled_area::parse()
{
	logger.info()() << "parsing controlled area description file: " << filename;

	try {
		using boost::property_tree::ptree;
		ptree pt;

		read_xml(filename, pt);

		BOOST_FOREACH(const ptree::value_type& v, pt.get_child("area")) {
			if (!parse_area(v.first, v.second)) {
				return false;
			}
		}
	} catch (std::exception& e) {
		logger.error()() << "parsing error: " << e.what();
		return false;
	}

	logger.info()() << "parsed controlled area description file: " << filename;
	return true;
}

bool controlled_area::parse_area(
		const std::string& key, const boost::property_tree::ptree& pt)
{
	bool res = true;

	if ("scope" == key) {
		data.scope = pt.get_value<int>();
		logger.debug()() << "controlled area scope is " << data.scope;
	} else if("actuators" == key) {
		res = parse_actuators(pt);
	} else if("dependencies" == key) {
		res = parse_dependencies(pt);
	} else if("collisions" == key) {
		res = parse_collisions(pt);
	}
	return res;
}

bool controlled_area::parse_actuators(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("actuator" == v.first) {
			if (!parse_actuator(v.second)) {
				return false;
			}
		}
	}
	return true;
}

bool controlled_area::parse_actuator(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	actuator_data act;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("name" == v.first) {
			act.name = v.second.get_value<std::string>();
		} else if ("exits" == v.first) {
			act.exits = v.second.get_value<int>();
		} else if ("before" == v.first) {
			BOOST_FOREACH(const ptree::value_type& v2, v.second) {
				detailed_data det;
				det.flow_sensor = v2.second.get_optional<std::string>("flow_sensor");
				det.queue_sensor = v2.second.get_optional<std::string>("queue_sensor");
				det.area = v2.second.get_optional<int>("area");
				act.before.push_back(det);
			}
		} else if ("after" == v.first) {
			BOOST_FOREACH(const ptree::value_type& v2, v.second) {
				detailed_data det;
				det.flow_sensor = v2.second.get_optional<std::string>("flow_sensor");
				det.queue_sensor = v2.second.get_optional<std::string>("queue_sensor");
				det.area = v2.second.get_optional<int>("area");
				act.after.push_back(det);
			}
		}
	}

	data.actuators.insert(std::make_pair(act.name, act));
	logger.debug()() << "created actuator " << act.name;
	return true;
}

bool controlled_area::parse_dependencies(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("actuator" == v.first) {
			std::string name = v.second.get<std::string>("name");
			if (data.actuators.end() == data.actuators.find(name)) {
				logger.error()() << "no actuator of given name: " << name;
				return false;
			}

			BOOST_FOREACH(const ptree::value_type& tree, v.second.get_child("depends_on")) {
				if ("name" == tree.first) {
					std::string name2 = tree.second.get_value<std::string>();
					if (data.actuators.end() == data.actuators.find(name2)) {
						logger.error()() << "no actuator of given name: " << name2;
						return false;
					}
					logger.debug()() << "actuator " << name << " depends on actuator " << name2;
					data.actuators.find(name2)->second.dependent.insert(name);
				}
			}
		}
	}
	return true;
}

bool controlled_area::parse_collisions(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("actuator" == v.first) {
			std::string name = v.second.get<std::string>("name");
			if (data.actuators.end() == data.actuators.find(name)) {
				logger.error()() << "no actuator of given name: " << name;
				return false;
			}
			actuator_data& act = data.actuators.find(name)->second;

			BOOST_FOREACH(const ptree::value_type& tree, v.second.get_child("collides_with")) {
				if ("collision" == tree.first) {
					std::string name2 = tree.second.get<std::string>("name");
					int cooldown = tree.second.get<int>("cooldown");

					logger.debug()() << "actuator " << name << " collides with actuator " << name2;
					act.collides.insert(std::make_pair(name2, cooldown));
				}
			}
		}
	}
	return true;
}

} // namespace controller