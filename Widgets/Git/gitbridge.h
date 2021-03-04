#ifndef GITBRIDGE_H
#define GITBRIDGE_H


/*
  * use Libgit2 API for that purposes, look it up
  * http://wiki.eclipse.org/Orion/Server_API/Git_API#git_clone
*/

#include <git2.h>

class GitBridge {
public:
    GitBridge();
    ~GitBridge() = default;

    void loadRepository();
    int commit(const char *comment);
    int push();
    void add();
    void getStatus();
    int clone();

    struct describe_options {
        const char **commits;
        size_t commit_count;
        git_describe_options describe_options;
        git_describe_format_options format_options;
    };

    void describe(describe_options *opts);


    git_repository *repo;
};

#endif // GITBRIDGE_H
