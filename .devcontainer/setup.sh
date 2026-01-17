apt-get update
apt-get install -y clang-21 clangd-21 llvm-21
apt-get install -y cmake git curl zstd libzstd-dev tree
mkdir -p /root/.ssh
ssh-keyscan github.com >> ~/.ssh/known_hosts