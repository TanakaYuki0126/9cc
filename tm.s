.intel_syntax noprefix
.global main
main:
    push 8
    push 4
    pop rdi
    pop rax
    add rax, rdi
    push rax
    pop rax
    ret