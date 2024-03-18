#include <stdio.h>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <sys/wait.h>

#include "../plugin.h"
#include "state.h"

using namespace plugin;

int main(int argc, char *argv[]) {

    (void) argc; (void) argv;

    auto state = State { 0 };

    const char *build_cmd[] = {
        "make",
        "lib.so",
        NULL,
    };

    for (;;)
    {
        int pid = fork();
        if (pid < 0)
        {
            throw std::runtime_error("fork failed");
        }
        else if (pid == 0)
        {
            // child
            if (execvp(build_cmd[0], (char**)build_cmd) < 0)
            {
                throw std::runtime_error("execvp failed: " + std::string(strerror(errno)));
            }
        }
        else
        {
            // parent
            int status;
            if (waitpid(pid, &status, 0) < 0)
            {
                throw std::runtime_error("waitpid failed: " + std::string(strerror(errno)));
            }

            if (!WIFEXITED(status))
            {
                throw std::runtime_error("plugin exited abnormally");
            }

            if (WEXITSTATUS(status) != 0)
            {
                throw std::runtime_error("child failed: " + std::to_string(WEXITSTATUS(status)));
            }
        }

        auto plugin = Plugin<State>("./lib.so", &state);

        auto ret = plugin.main();

        if (ret) {
            return ret;
        }

        printf("state->number: %d\n", state.number);
        printf("Edit lib.cc and press ENTER\n");
        getc(stdin);
    }

    return 0;
}
