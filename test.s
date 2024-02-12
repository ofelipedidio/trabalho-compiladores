.text
.globl n
.bss
.align 4
.type n, @object
.size n, 4
n:
.zero 4
.globl f
.bss
.align 4
.type f, @object
.size f, 4
f:
.zero 4
.globl b
.bss
.align 4
.type b, @object
.size b, 4
b:
.zero 4
.text
.globl main
.type main, @function
main:
endbr64
pushq %rbp
movq %rsp, %rbp
// lit (int)
movl $15, %eax
// assignment (n)
movl %eax, n(%rip)
// eq (left)
// mod (left)
// ident
movl n(%rip), %eax
pushq %rax
// mod (right)
// lit (int)
movl $3, %eax
movl %eax, %ecx
popq %rax
// mod (op)
cltd
idivl %ecx
movl %edx, %eax
pushq %rax
// eq (right)
// lit (int)
movl $0, %eax
popq %rbx
// eq (cmp)
cmp %ebx, %eax
jne L5
movl $1, %eax
jmp L6
L5:
movl $0, %eax
// eq (done)
L6:
// assignment (f)
movl %eax, f(%rip)
// eq (left)
// mod (left)
// ident
movl n(%rip), %eax
pushq %rax
// mod (right)
// lit (int)
movl $5, %eax
movl %eax, %ecx
popq %rax
// mod (op)
cltd
idivl %ecx
movl %edx, %eax
pushq %rax
// eq (right)
// lit (int)
movl $0, %eax
popq %rbx
// eq (cmp)
cmp %ebx, %eax
jne L9
movl $1, %eax
jmp L10
L9:
movl $0, %eax
// eq (done)
L10:
// assignment (b)
movl %eax, b(%rip)
// Return
// add (left)
// mul (left)
// lit (int)
movl $2, %eax
pushq %rax
// mul (right)
// ident
movl f(%rip), %eax
popq %rbx
// mul (op)
imull %ebx, %eax
pushq %rax
// add (right)
// ident
movl b(%rip), %eax
popq %rbx
// add (op)
addl %ebx, %eax
popq %rbp
ret
movl $1, %eax
popq %rbp
ret
.section .note.GNU-stack,"",@progbits
