#include "gitbridge.h"
#include <QDebug>
#include <QSettings>
#include <stdio.h>

GitBridge::GitBridge() {
    //loadRepository();
}

void GitBridge::loadRepository() {
    QSettings settings("Evolution-IDE");
    const QString path = settings.value("Evolution-IDE/git_paht").toString();
    const int error = git_repository_open(&repo, path.toLatin1().data());
    if (error != 0) {
        // 0 is good
        qDebug() << "cannot load repository dir !";
    }
}

int GitBridge::commit(const char *comment) {
    int error;

    git_oid commit_oid, tree_oid;
    git_tree *tree;
    git_index *index;
    git_object *parent = NULL;
    git_reference *ref = NULL;
    git_signature *signature;

    error = git_revparse_ext(&parent, &ref, repo, "HEAD");
    if (error == GIT_ENOTFOUND) {
        printf("HEAD not found. Creating first commit\n");
        error = 0;
    } else if (error != 0) {
        const git_error *err = git_error_last();
        if (err) printf("ERROR %d: %s\n", err->klass, err->message);
        else
            printf("ERROR %d: no detailed info\n", error);
    }

    git_repository_index(&index, repo);
    git_index_write_tree(&tree_oid, index);
    git_index_write(index);

    git_tree_lookup(&tree, repo, &tree_oid);

    git_signature_default(&signature, repo);

    git_commit_create_v(
            &commit_oid,
            repo,
            "HEAD",
            signature,
            signature,
            NULL,
            comment,
            tree,
            parent ? 1 : 0, parent);

    git_index_free(index);
    git_signature_free(signature);
    git_tree_free(tree);

    return error;
}


void GitBridge::describe(describe_options *opts) {
    if (opts->commit_count == 0) {
        //do_describe_single(repo, opts, NULL);
    } else {
        size_t i;
        for (i = 0; i < opts->commit_count; i++) {
            //do_describe_single(repo, opts, opts->commits[i]);
        }
    }
}

int GitBridge::push() {
    git_push_options options;
    git_remote *remote = NULL;
    char *refspec = "refs/heads/master";
    const git_strarray refspecs = {
            &refspec,
            1};

    git_remote_lookup(&remote, repo, "origin");

    git_push_options_init(&options, GIT_PUSH_OPTIONS_VERSION);

    git_remote_push(remote, &refspecs, &options);

    printf("push exited successfully");
    return 0;
}
