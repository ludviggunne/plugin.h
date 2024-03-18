#include "state.h"
#include "../plugin.h"

static int plugin_main(void *state) {
    auto state_ptr = static_cast<State*>(state);
    state_ptr->number--;
    return 0;
}

PLUGIN_MAIN(plugin_main)
