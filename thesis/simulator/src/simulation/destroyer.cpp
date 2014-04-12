#include "destroyer.h"

namespace simulator {
namespace simulation {

destroyer::destroyer(cell_handle cell_, int max_destroy_rate_) :
	cell(cell_),
	max_destroy_rate(max_destroy_rate_)
{}

}
}