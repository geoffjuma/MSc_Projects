@ This file is part of the examples for the Intro to ARM course.
@ This was derived from Peter Nelson's blog at http://peterdn.com/post/e28098Hello-World!e28099-in-ARM-assembly.aspx.
@ For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .

.text
_start: .global _start
@ sys_write ( fd, pstr, len)
@ r7=4 r0 r1 r2
mov r0, #1 @ fd <- stdout
adr r1, msg @ pstr <- *msg
mov r2, #14 @ len <- 14
mov r7, #4 @ syscall <- sys_write
swi 0 @ system call
@ sys_exit ( exitcode )
@ r7=1 r0
mov r0, #0 @ exitcode <- 0
mov r7, #1 @ syscall <- sys_exit
swi 0 @ system call
msg:
.asciz "Hello, world!\n"
.end

