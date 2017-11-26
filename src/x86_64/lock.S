.intel_syntax noprefix

.section .text

# acquire_spinlock
.global acquire_spinlock
.type acquire_spinlock, @function
spin_wait:
	testb [esp + 4], 1
	jnz spin_wait
acquire_spinlock:
	lock orb [esp + 4], 0
	jc spin_wait
	ret

# release_spinlock
.global release_spinlock
.type release_spinlock, @function
release_spinlock:
	mov eax, [esp+4]
	movb [esp+4], 0
	ret
