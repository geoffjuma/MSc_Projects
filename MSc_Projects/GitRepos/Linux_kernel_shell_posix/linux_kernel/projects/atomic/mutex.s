@ This file is part of the atomic lab and is derived based on the code from ARM's website at:
@ http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dht0008a/ch01s03s02.html .
@ For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .
@ License: Creative Commons: Attribution, Share-Alike license
@ For details please refer to http://creativecommons.org/licenses/by-sa/3.0/.

	.syntax unified
	.arch armv7-a
	.text

	.equ locked, 1
	.equ unlocked, 0

	.global lock_mutex
	.type lock_mutex, function
lock_mutex:
        @ INSERT CODE BELOW

        @ END CODE INSERT
	bx lr

	.size lock_mutex, .-lock_mutex

	.global unlock_mutex
	.type unlock_mutex, function
unlock_mutex:
	@ INSERT CODE BELOW
        
        @ END CODE INSERT
	bx lr
	.size unlock_mutex, .-unlock_mutex

	.end
