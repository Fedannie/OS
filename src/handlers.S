.text
.code64
.global handlers

#define NoErrHandler(i) \
handler##i: \
		pushq $0; \
		pushq $i; \
		jmp AnyHandler \

#define ErrHandler(i) \
handler##i: \
		pushq $i; \
		jmp AnyHandler \

NoErrHandler(0)
NoErrHandler(1)
NoErrHandler(2)
NoErrHandler(3)
NoErrHandler(4)
NoErrHandler(5)
NoErrHandler(6)
NoErrHandler(7)
ErrHandler(8)
NoErrHandler(9)
ErrHandler(10)
ErrHandler(11)
ErrHandler(12)
ErrHandler(13)
ErrHandler(14)
NoErrHandler(15)
NoErrHandler(16)
ErrHandler(17)
NoErrHandler(18)
NoErrHandler(19)
NoErrHandler(20)
NoErrHandler(21)
NoErrHandler(22)
NoErrHandler(23)
NoErrHandler(24)
NoErrHandler(25)
NoErrHandler(26)
NoErrHandler(27)
NoErrHandler(28)
NoErrHandler(29)
NoErrHandler(30)
NoErrHandler(31)
NoErrHandler(32)
NoErrHandler(33)
NoErrHandler(34)
NoErrHandler(35)
NoErrHandler(36)
NoErrHandler(37)
NoErrHandler(38)
NoErrHandler(39)
NoErrHandler(40)
NoErrHandler(41)
NoErrHandler(42)
NoErrHandler(43)
NoErrHandler(44)
NoErrHandler(45)
NoErrHandler(46)
NoErrHandler(47)
NoErrHandler(48)
NoErrHandler(49)

handlers:
	.quad handler0
	.quad handler1
	.quad handler2
	.quad handler3
	.quad handler4
	.quad handler5
	.quad handler6
	.quad handler7
	.quad handler8
	.quad handler9
	.quad handler10
	.quad handler11
    .quad handler12
	.quad handler13
	.quad handler14
	.quad handler15
	.quad handler16
	.quad handler17
	.quad handler18
	.quad handler19
	.quad handler20
	.quad handler21
	.quad handler22
	.quad handler23
	.quad handler24
	.quad handler25
	.quad handler26
	.quad handler27
	.quad handler28
	.quad handler29
	.quad handler30
	.quad handler31
		.quad handler32
	.quad handler33
	.quad handler34
	.quad handler35
    .quad handler36
	.quad handler37
	.quad handler38
	.quad handler39
		.quad handler40
	.quad handler41
	.quad handler42
	.quad handler43
	.quad handler44
	.quad handler45
	.quad handler46
	.quad handler47
	.quad handler48
	.quad handler49

AnyHandler:
	pushq %rdi
	movq 8(%rsp), %rdi
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rbp
	pushq %rsi
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	cld                   //clear direction flag
	call interruption
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rsi
	popq %rbp
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax
	popq %rdi
	addq $16, %rsp
	iretq