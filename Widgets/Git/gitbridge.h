#ifndef GITBRIDGE_H
#define GITBRIDGE_H


/*
  * use Libgit2 API for that purposes, look it up
  * http://wiki.eclipse.org/Orion/Server_API/Git_API#git_clone
  * main resource:  https://libgit2.org/docs/guides/101-samples/
*/

#include <git2.h>
#include <iostream>
#include <string>
#include <vector>

class GitBridge {
public:
    GitBridge();

    ~GitBridge();

    // libgit2 keeps some data in thread-local storage:
    // this functions handles all errors that came out of function calls
    // with appropriate description + appends git error state, message
    void errorManager(const int& error, const char* desc);

    // set free all resources used by git API to avoid memory leaks
    // and terminates library from memory
    void freeResources();

    void loadRepository();

    int commit(const char *comment);

    int push(char *refspec = "refs/heads/master");

    struct describe_options {
        const char **commits;
        size_t commit_count;
        git_describe_options describe_options;
        git_describe_format_options format_options;
    };

    void describe(describe_options *opts);

    struct initOpts {
        int no_options;
        int quiet;
        int bare;
        int initial_commit;
        uint32_t shared;
        const char *Template;
        const char *gitdir;
        const char *dir;
    };

    int init(const char *RepoName, const bool &initCommit);

    void initialCommit();


    struct index_options {
        int dry_run;
        int verbose;
        git_repository *repo;
        /*some enum*/int mode;
        int add_update;
    };

    int add(const char *file);

    int remove(const char *file);

    typedef struct progress_data {
        git_indexer_progress fetch_progress;
        size_t completed_steps;
        size_t total_steps;
        const char *path;
    } progress_data;

    int clone(const char *dest, const char *url);

    // same as pull
    int fetch(const char *repo_name);

    int status();

    const char *findRepo(const char *repo_name);

    git_repository *repo;

    const char *getBranch();

    void getStatusData(git_status_list *status);

private:

};

#endif // GITBRIDGE_H
