.text
.text
.globl main
.type main, @function
main:
endbr64
pushq %rbp
movq %rsp, %rbp
movq $16, %rax
subq %rax, %rbp
// lit (int)
movl $2, %eax
// assignment (z)
movl %eax, -12(%rbp)
// lit (int)
movl $4, %eax
// assignment (b)
movl %eax, -4(%rbp)
// lit (int)
movl $9, %eax
// assignment (c)
movl %eax, -8(%rbp)
// inv (expr)
// lit (int)
movl $4, %eax
// inv (op)
negl %eax
// assignment (a)
movl %eax, -0(%rbp)
// if (expr)
// gt (left)
// ident
movl -0(%rbp), %eax
pushq %rax
// gt (right)
// lit (int)
movl $0, %eax
popq %rbx
// gt (cmp)
cmp %eax, %ebx
jg L7
movl $0, %eax
jmp L8
L7:
movl $1, %eax
// gt (done)
L8:
// if (cmp)
and %eax, %eax
jz L11
// if (then)
// lit (int)
movl $1, %eax
// assignment (c)
movl %eax, -8(%rbp)
// add (left)
// ident
movl -12(%rbp), %eax
pushq %rax
// add (right)
// ident
movl -8(%rbp), %eax
popq %rbx
// add (op)
addl %ebx, %eax
// assignment (b)
movl %eax, -4(%rbp)
// if (done)
L11:
// Return
// ident
movl -4(%rbp), %eax
popq %rbp
ret
movl $1, %eax
popq %rbp
ret
.section .note.GNU-stack,"",@progbits
