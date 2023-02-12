sudo losetup -fP loopbackfile1.img
sudo losetup -fP loopbackfile2.img

losetup -a|grep file

# mount -o loop -t tfs /dev/loop21 /loopfs
