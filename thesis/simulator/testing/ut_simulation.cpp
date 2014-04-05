#include <gtest/gtest.h>

#include "../src/world/world_description.h"
#include "../src/simulation/simulation.h"

namespace simulator {
namespace simulation {
namespace constant {
	const std::string filename("dummy_filename");

	const std::string node_a_name("NODE_A");
	const world::ENodeType node_a_type(world::ENodeType_Entrance);
	const int node_a_max_create(100);

	const std::string node_b_name("NODE_B");
	const world::ENodeType node_b_type(world::ENodeType_Split);

	const std::string node_c_name("NODE_C");
	const world::ENodeType node_c_type(world::ENodeType_Exit);
	const int node_c_max_destroy(50);

	const std::string node_d_name("NODE_D");
	const world::ENodeType node_d_type(world::ENodeType_Exit);
	const int node_d_max_destroy(75);

	const int distance_a_b(20);
	const int distance_b_c(25);
	const int distance_b_d(15);

	const double cell_size(7.5);
}

class ut_simulation : public ::testing::Test
{
protected:
	simulation sim;

	ut_simulation() :
		sim(constant::filename)
	{}

	world::world_description_handle prepare_t_shape_desc()
	{
		world::world_description_handle desc(new world::world_description());

		world::world_node_handle node_a(new world::world_node(constant::node_a_name, constant::node_a_type));
		node_a->max_create_rate = constant::node_a_max_create;
		desc->nodes.insert(std::make_pair(constant::node_a_name, node_a));

		world::world_node_handle node_b(new world::world_node(constant::node_b_name, constant::node_b_type));
		desc->nodes.insert(std::make_pair(constant::node_b_name, node_b));

		world::world_node_handle node_c(new world::world_node(constant::node_c_name, constant::node_c_type));
		node_c->max_destroy_rate = constant::node_c_max_destroy;
		desc->nodes.insert(std::make_pair(constant::node_c_name, node_c));

		world::world_node_handle node_d(new world::world_node(constant::node_d_name, constant::node_d_type));
		node_d->max_destroy_rate = constant::node_d_max_destroy;
		desc->nodes.insert(std::make_pair(constant::node_d_name, node_d));

		world::world_connection_handle conn_a_b(new world::world_connection(constant::distance_a_b));
		node_a->exits[0] = conn_a_b;
		node_b->entrances[0] = conn_a_b;
		desc->connections.insert(conn_a_b);

		world::world_connection_handle conn_b_c(new world::world_connection(constant::distance_b_c));
		node_b->exits[0] = conn_b_c;
		node_c->entrances[0] = conn_b_c;
		desc->connections.insert(conn_b_c);

		world::world_connection_handle conn_b_d(new world::world_connection(constant::distance_b_d));
		node_b->exits[1] = conn_b_d;
		node_d->entrances[0] = conn_b_d;
		desc->connections.insert(conn_b_d);

		desc->simulation.reset(new world::world_simulation());
		desc->simulation->cell_size = constant::cell_size;

		return desc;
	}

	bool trigger_translate_nodes(world::world_description_handle desc)
	{
		return sim.translate_nodes(desc);
	}

	const std::set<cell_handle>& get_cells()
	{
		return sim.cells;
	}

	const std::map<std::string, cell_handle>& get_cell_names()
	{
		return sim.cell_names;
	}
};

TEST_F(ut_simulation, translate_to_cell_representation)
{
	world::world_description_handle desc = prepare_t_shape_desc();
	ASSERT_EQ(4, desc->nodes.size());
	ASSERT_EQ(3, desc->connections.size());

	EXPECT_TRUE(trigger_translate_nodes(desc));

	const std::set<cell_handle>& cells = get_cells();
	const std::map<std::string, cell_handle>& cell_names = get_cell_names();

	EXPECT_EQ(4, cells.size());
	EXPECT_EQ(4, cell_names.size());
}

} // namespace simulation
} // namespace simulator