sudo apt install build-essential libreadline-dev

mkdir lua_build
cd lua_build
curl -R -O http://www.lua.org/ftp/lua-5.4.7.tar.gz
tar zxf lua-5.4.7.tar.gz
cd lua-5.4.7
make linux test
sudo make install