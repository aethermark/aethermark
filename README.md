sudo apt update
sudo apt install -y g++ make python3-dev python3-venv \
 libgtest-dev cmake
cd /usr/src/googletest
sudo cmake .
sudo make
sudo cp lib/\*.a /usr/lib

pip install --index-url https://test.pypi.org/simple/ \
 --extra-index-url https://pypi.org/simple \
 aethermark==0.1.2
