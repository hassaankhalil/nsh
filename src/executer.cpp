#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;

    std::vector<const char *> argv;

    for (const std::string &token : tokens)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid == 0)
    {
        int status = execvp(argv[0], const_cast<char *const *>(argv.data()));

        if (status != 0)
        {
            if (errno == ENOENT)
                std::cerr << tokens[0] << ": command not found" << std::endl;
            else
                std::cerr << tokens[0] << ": failed to execute command" << std::endl;
        }
    }
    else if (pid == -1)
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else
        waitpid(pid, nullptr, 0);
}
