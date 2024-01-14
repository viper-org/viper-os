cd $(dirname $0)

git clone https://github.com/limine-bootloader/limine.git --branch=v5.x-branch-binary --depth=1
make -C limine

mkdir -p ../../initrd
make -C ../../vipera
mv ../../vipera/build/* ../../initrd

cd ../../initrd
tar cvf ../modules/initrd.tar *
cd ../atheris/x64

rm -f viperos-x64.hdd
dd if=/dev/zero bs=1M count=0 seek=64 of=viperos-x64.hdd
sgdisk viperos-x64.hdd -n 1:2048 -t 1:ef00
./limine/limine bios-install viperos-x64.hdd
mformat -i viperos-x64.hdd@@1M
mmd -i viperos-x64.hdd@@1M ::/EFI ::/EFI/BOOT
mcopy -i viperos-x64.hdd@@1M ./viperos-x64 ../../modules/* limine.cfg limine/limine-bios.sys ::/
mcopy -i viperos-x64.hdd@@1M limine/BOOTX64.EFI ::/EFI/BOOT
mcopy -i viperos-x64.hdd@@1M limine/BOOTIA32.EFI ::/EFI/BOOT