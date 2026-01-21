# echo "precedence ::ffff:0:0/96  100" >> /etc/gai.conf
apt-get update
apt-get install -y clang-21  clangd-21 
apt-get install -y llvm-21 llvm-21-dev
apt-get install -y lldb-21 gdb
apt-get install -y cmake  git  curl  zstd libzstd-dev  tree libedit-dev libcurl4-openssl-dev

# apt-get install -y \
#   ca-certificates \
#   git \
#   curl \
#   gnupg \
#   lsb-release \
#   software-properties-common

  
# curl -fsSL https://deb.nodesource.com/setup_20.x | bash -
# apt-get install -y nodejs

ssh-keyscan github.com >> ~/.ssh/known_hosts
