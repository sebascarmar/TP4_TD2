.text
.arm
.global operaciones

//;R0: operando1 / R1: operando2 / R2: operación 0 -> suma 
//;                                              1 -> resta
//;                                              2 -> and lógica
//;                                              3 -> or lógica
//;                                              X -> suma por default

operaciones:
    cmp r2, #0
    bne resta
    add r0, r0, r1
    b salir
  
  resta:
    cmp r2, #1
    bne andd
    sub r0, r0, r1
    b salir
  
  andd:
    cmp r2, #2
    bne or
    and r0, r0, r1
    b salir

  or:
    cmp r2, #3
    bne default
    orr r0, r0, r1
    b salir

  default:
    add r0, r0, r1
    b salir

  salir:
    mov pc, lr
    
