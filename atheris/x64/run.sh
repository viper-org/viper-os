cd $(dirname $0)

mkdir -p ovmf
cd ovmf && curl -Lo OVMF.fd https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF.fd

cd ..

qemu-system-x86_64 -M q35 -m 2G -bios ovmf/OVMF.fd -hda ./viperos-x64.hdd