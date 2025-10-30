cd $(dirname $0)

git clone https://github.com/limine-bootloader/limine.git --branch=v5.x-branch-binary --depth=1
make -C limine

mkdir -p ../drivers/bin
mkdir -p ../modules
mkdir -p ../initrd/boot/
mkdir -p ../initrd/bin/
cp -r ../drivers/bin ../initrd/boot
cp -r ../user/sysroot/* ../initrd/
cd ../initrd
tar cvf ../modules/initrd.tar **
cd ../krnl

rm -f viperos-amd64.hdd
dd if=/dev/zero bs=1M count=0 seek=64 of=viperos-amd64.hdd
sgdisk viperos-amd64.hdd -n 1:2048 -t 1:ef00
./limine/limine bios-install viperos-amd64.hdd
mformat -i viperos-amd64.hdd@@1M
mmd -i viperos-amd64.hdd@@1M ::/EFI ::/EFI/BOOT
mcopy -i viperos-amd64.hdd@@1M ./viperos-amd64 limine.cfg limine/limine-bios.sys ../modules/initrd.tar ::/
mcopy -i viperos-amd64.hdd@@1M limine/BOOTX64.EFI ::/EFI/BOOT
mcopy -i viperos-amd64.hdd@@1M limine/BOOTIA32.EFI ::/EFI/BOOT