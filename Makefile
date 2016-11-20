all:
	cd src
	make all
	mv kernel ../build/iso/boot
clean:
	cd build/
	rm NORSE.iso /iso/boot/kernel
