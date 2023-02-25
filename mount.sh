
#sudo insmod build/kmod/tfs_kmod.ko 
sudo insmod ./cmake-build-release/kmod/tfs_kmod.ko

sudo mount -o loop -o dummy1 -t tfs /dev/loop1 /loopfs -O workme
