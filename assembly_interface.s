global outb

outb:
    mov al, [esp+8]
    mov dx, [esp+4]
    out dx, al
    ret

global inb

inb:
    mov dx, [esp+4]
    in al, dx
    ret

global lgdt

lgdt:
    mov edx, [esp+4]
    lgdt [edx]
    ret

global load_idt

load_idt:
   mov eax, [esp+4]
   lidt [eax]
   ret

global interrupt

interrupt:
    mov eax, [esp+4]
    int 9

global sti

sti:
  sti
  ret
