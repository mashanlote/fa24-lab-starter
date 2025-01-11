.globl f # this allows other files to find the function f

# f takes in two arguments:
# a0 is the value we want to evaluate f at
# a1 is the address of the "output" array (read the lab spec for more information).
# The return value should be stored in a0
f:
    # Your code here 
    addi t0 a0 3 # translate value to offset

    addi t1 x0 4 # s
    mul t1 t1 t0 # calculate offset
    add t1 a1 t1 # calculate address at offset

    lw t1 0(t1)
    mv a0 t1
    
    # This is how you return from a function. You'll learn more about this later.
    # This should be the last line in your program.
    jr ra
