## Anders Dahl 
## 11:35 Section

.globl main
.data
	x: .word 0
	string1: .asciiz "Positive Integer: "
	string2: .asciiz "The value of 'factorial("
	string3: .asciiz ")' is: "
	
.text

main:

## printf("Positive integer: ");
la $a0, string1				# Load the value of string1 into $a0
jal print_mystring			# Call print_mystring(string1)

## scanf("%d", &number);
li $v0, 5				# Input of an integer is number 5, argument already
					# in $a0
syscall					# Calls the system
move $t0, $v0				# $t0 = $v0

## printf("The value of 'factorial);
la $a0, string2				# Same thing as before, but with a different string
jal print_mystring

## printf("%d", number);
move $a0, $t0				# $a0 = $t0
jal print_myinteger			# Call print_myinteger($a0)

## factorial (number);
jal factorial				# Call factorial($a0)
move $t1, $v0

## printf(")' is:  ");
la $a0, string3				# Same thing as before, but with a different string
jal print_mystring

## printf("%d", factorial (number));
lw $a0, x				# Load the value from x into $a0
jal print_myinteger

## exit
li $v0, 10				# Exiting is system call 10
syscall					# System call

## prints a string
print_mystring:				# Prints a string
addi $sp, $sp, -4			# Extend stack; Use -4 because stack grows downwards
sw $ra, 0($sp)				# Store $ra on stack
li $v0, 4				# Printing a string is input 4, argument already
					# in $a0
syscall					# System call
li $v0, 0				# 0 means success; Pass return value in $v0

lw $ra, 0($sp)				# Load $ra from stack
addi $sp, $sp, 4			# Contract stack to where it used to be

jr $ra

## prints an integer
print_myinteger:			# Prints an integer
addi $sp, $sp, -4			# The same general idea is used when calling functions
sw $ra, 0($sp)
li $v0, 1				# Printing an integer is input 1, argument already
					# in $a0
syscall					# System call
lw $ra, 0($sp)
addi $sp, $sp, 4
jr $ra

factorial:				# Finds the factorial of an integer
## if (x == 0)
beqz $a0, else				# If equal to zero, go to else

## return x * factorial(x-1);
addi $sp, $sp, -16			# Extend stack; Use -4 because stack grows downwards
sw $ra, 0($sp)				# Store $ra on stack
sw $a0, 4($sp)				# Store $a0 on stack

move $t5, $a0				# $t5 = $a0
addi $t5, $t5, -1			# $t5 = $t5 - 1
move $a0, $t5				# $a0 = $t5

jal factorial				# Jumps and links to factorial, basically calls
					# factorial again

move $t6, $v0				# $t6 = $v0
lw $t7, 4($sp)				# Load $t7 from stack; the argument stored
mult $t7, $t6				# lo = $t7 * $t6
mflo $t8				# $t8 = lo
move $v0, $t8				# $v0 = $t8
sw $v0, x($zero)			# Store the value in $v0 in x
lw $ra, 0($sp)				# Load in the previous $ra
addi $sp, $sp, 16			# Contract stack to where it used to be before you saved
					# the $ra
jr $ra					# Return $ra

## return 1;
else:
addi $t9, $zero, 1			# $t9 = $0 + 1
move $v0, $t9				# $v0 = $t9
sw $v0, x($zero)			# v0 is the return variable
jr $ra					# Return $ra
