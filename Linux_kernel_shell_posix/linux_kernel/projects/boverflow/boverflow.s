@ This file is part of the control flow hijack lab.
@ For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .
@ License: Creative Commons: Attribution, Share-Alike license
@ For details please refer to http://creativecommons.org/licenses/by-sa/3.0/.
	.syntax unified
	.arch armv7-a
	.eabi_attribute 27, 3
	.fpu vfpv3-d16
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.thumb
	.file	"boverflow.c"
	.text
	.align	2
	.global	main
	.thumb
	.thumb_func
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #8
	add	r7, sp, #0
	str	r0, [r7, #4]
	str	r1, [r7, #0]
	ldr	r3, [r7, #0]
	add	r3, r3, #4
	ldr	r3, [r3, #0]
	mov	r0, r3
	bl	stage1
	mov	r3, #0
	mov	r0, r3
	add	r7, r7, #8
	mov	sp, r7
	pop	{r7, pc}
	.size	main, .-main
	.section	.rodata
	.align	2
.LC0:
	.ascii	"%s\000"
	.text
	.align	2
	.global	stage1
	.thumb
	.thumb_func
	.type	stage1, %function
stage1:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #24
	add	r7, sp, #0
	str	r0, [r7, #4]
	movw	r3, #:lower16:__stack_chk_guard
	movt	r3, #:upper16:__stack_chk_guard
	ldr	r3, [r3, #0]
	str	r3, [r7, #20]
	add	r3, r7, #12
	mov	r0, r3
	ldr	r1, [r7, #4]
	bl	strcpy
	movw	r3, #:lower16:.LC0
	movt	r3, #:upper16:.LC0
	add	r2, r7, #12
	mov	r0, r3
	mov	r1, r2
	bl	printf
	movw	r3, #:lower16:__stack_chk_guard
	movt	r3, #:upper16:__stack_chk_guard
	ldr	r2, [r7, #20]
	ldr	r3, [r3, #0]
	cmp	r2, r3
	beq	.L3
	bl	__stack_chk_fail
.L3:
	add	r7, r7, #24
	mov	sp, r7
	pop	{r7, pc}
	.size	stage1, .-stage1
	.ident	"GCC: (Ubuntu/Linaro 4.6.1-9ubuntu3) 4.6.1"
	.section	.note.GNU-stack,"",%progbits
