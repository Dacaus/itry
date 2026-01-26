apt-get update
apt-get upgrade -y
apt-get install -y clang-21  clangd-21 
apt-get install -y llvm-21 llvm-21-dev
apt-get install -y lldb-21 gdb
apt-get install -y cmake  git  curl  zstd libzstd-dev  tree libedit-dev libcurl4-openssl-dev
apt-get install -y debuginfod


ssh-keyscan github.com >> ~/.ssh/known_hosts