set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/norse.kernel isodir/boot/norse.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "norse" {
	multiboot /boot/norse.kernel
}
EOF
grub2-mkrescue -o norse.iso isodir
