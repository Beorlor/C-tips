The union is used here to represent a hardware register
of a microcontroller or other embedded system component. The unsigned int reg
allows for accessing the entire 32-bit register at once, while the struct with
control, direction, parity, and stopbits allows for accessing individual bytes
of the register. This is crucial for embedded systems where different bits or
groups of bits in a hardware register control different functionalities.

The ability to access and manipulate either the whole register or its individual
parts (bytes) easily is a significant advantage. For instance,
setting gpioa.reg = 0x41424344; initializes all parts of the register at once.
Later, you can read or modify individual parts like control or stopbits without
affecting other bytes.
This is particularly useful when different bits/bytes of a register are
responsible for different functions.

#include <stdio.h>

union HWRegister
{
	struct
	{
		unsigned char	control;
		unsigned char	direction;
		unsigned char	parity;
		unsigned char	stopbits;
	};
	unsigned int	reg;
};

int	main(void)
{
	union HWRegister	gpioa;

	gpioa.reg = 0x41424344;

	printf("%x\n", gpioa.stopbits); // print 41

	printf("%x\n", gpioa.control); // print 44

	return (0);
}
