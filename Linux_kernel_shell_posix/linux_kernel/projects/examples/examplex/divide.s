@ This file is part of the examples for the Intro to ARM course.
@ For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .
.text
.global divide

divide:
  SDIV r0, r0, r1
  BX LR
  NOP

.global unsigneddivide
unsigneddivide:
  UDIV r0, r0, r1
  BX LR
  NOP
.end
