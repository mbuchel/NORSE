if grub2-file --is-x86-multiboot $1; then
	echo "Has multiboot"
else
	echo "No multiboot"
fi
