Contribution:

:: NOTE ::
- decompress it here, path in cmake are configured to look there.
- contains clang, llvm, lldb, git, hunspell libraries

- maximum github file size is 100 MB, therefore 7 lib files...
  just select them all and press decompress all (OS optional)










# lfs needed for large file transfer

# get into #PATH
git lfs install

# track this file
git lfs track '*.tag.gz'
git lfs track '*.zip'

# create gitattribute file and push pointers
git lfs push --all origin main

# then add this file to be tracked
git add .gitattributes
git commit -m "msg" .gitattributes
git push origin main
