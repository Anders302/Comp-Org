## Anders Dahl 
## 11:35 Section

# Note it works, but it takes a while to load the sorted list.
# Like around 40 seconds or so.

.globl main

.data

beginning: .asciiz "Initial array is:\n"
stringFront: .asciiz "["
stringEnd: .asciiz "]\n"
last: .ascii "Insertion sort is finished!\n"

array: .align 5
	.asciiz ""
	.align 5
	.asciiz "Joe "
	.align 5
	.asciiz "Jenny "
	.align 5
	.asciiz "Jill "
	.align 5
	.asciiz "John "
	.align 5
	.asciiz "Jeff "
	.align 5
	.asciiz "Joyce "
	.align 5
	.asciiz "Jerry "
	.align 5
	.asciiz "Janice "
	.align 5
	.asciiz "Jake "
	.align 5
	.asciiz "Jonna "
	.align 5
	.asciiz "Jack "
	.align 5
	.asciiz "Jocelyn "
	.align 5
	.asciiz "Jessie "
	.align 5
	.asciiz "Jess "
	.align 5
	.asciiz "Janet "
	.align 5
	.asciiz "Jane "
	.align 5
data:	.space 68 # 16 pointers to strings: 16 * 4 = 64

# Int size = 16
size:	.word 16

.text

main:

lw $s0, size		# $s0 = size
addi $s1, $zero, 1	# $s1 = 1

move $a0, $s0		# $a0 = size
jal initalize		# initalize(size);

la $a0, beginning	# print "Initial array is:\n"
jal printMyString

la $a0, stringFront	# print [
jal printMyString

move $a0, $s1		# $a0 = 1
jal print		# print_array(data, 1)
			# Prints the array until
			# the size of the array is
			# reached
			
la $a0, stringEnd	# print ]
jal printMyString

			# test str_lt
mul $t0, $s1, 4
mul $t1, $s1, 8
lw $t0, data($t0)
lw $t1, data($t1)
move $a0, $t0
move $a1, $t1
move $a2, $zero
jal str_lt

			# test innerFor
addi $a0, $zero, 3
jal outerFor

back:

la $a0, last		# print "Initial array is:\n"
jal printMyString

la $a0, stringFront	# print [
jal printMyString

move $a0, $s1		# $a0 = 1
jal print		# print_array(data, 1)
			# Prints the array until
			# the size of the array is
			# reached

la $a0, stringEnd	# print ]
jal printMyString

li $v0, 10		# Exits the program
syscall

#   char * data[] = {"Joe", "Jenny", "Jill", "John", "Jeff", "Joyce",
#		"Jerry", "Janice", "Jake", "Jonna", "Jack", "Jocelyn",
#		"Jessie", "Jess", "Janet", "Jane"};

initalize:		# Initalizes the data by adding the words
			# in the array to it
addi $sp, $sp, -4	# Extend stack; Use -4 because stack grows downwards
sw $ra, 0($sp)		# Store $ra on stack

move $t0, $a0		# $t0 = $a0
beqz  $t0, end		# Branch if $t0 = 0

mul $t1, $t0, 32	# $t1 = $t0 * 32
mul $t2, $t0, 4		# $t2 = $t0 * 4

la $t1, array($t1)	# $t1 = array[$t1 * 32]
sw $t1, data($t2)	# data[$t2 * 4] = $t1

addi $t0, $t0, -1	# $t0 = $t0 - 1
move $a0, $t0		# $a0 = $t0
jal initalize		# Calls initalize

lw $ra, 0($sp)		# Load $ra from stack
addi $sp, $sp, 4	# Contract stack to where it used to be
jr $ra			# Jump and return to $ra

print:
addi $sp, $sp, -4	# Extend stack and store $ra
sw $ra, 0($sp)

move $t0, $a0		# $t0 = $a0
move $t1, $s0		# $t1 = size

bgt $t0, $t1, end	# Branch if $t0 > size

mul $t2, $t0, 4		# $t2 = $t0 * 4
lw $a0, data($t2)	# $a0 = data[$t2 * 4]

jal printMyString	# print data[$t2 * 4]

addi $t2, $t0, 1	# $t2 = $t0 + 1
move $a0, $t2		# $a0 = $t2

jal print		# Calls print

lw $ra, 0($sp)		# Load $ra from stack
addi $sp, $sp, +4	# Contract stack to where it used to be
jr $ra			# Jump and return to $ra

end:
jr $ra

printMyString:		# Print the string
addi $sp, $sp, -4	# Extend stack and store $ra
sw $ra, 0($sp)

li $v0, 4		# System call for printing a string
syscall			# Call system

lw $ra, 0($sp)		# Load $ra from stack
addi $sp, $sp, +4	# Contract stack to where it used to be
jr $ra			# Jump and return to $ra

str_lt:
addi $sp, $sp, -4	# Extend stack and store $ra
sw $ra, 0($sp)

move $t0, $a0		# $t0 = $a0
move $t1, $a1		# $t1 = $a1
move $t2, $a2		# $t2 = $a2

add $t0, $t0, $t2	# $t0 = $t0 + $t2
add $t1, $t1, $t2	# $t1 = $t1 + $t2

lb $t0, ($t0)		# Load the byte that the 		
lb $t1, ($t1)		# address points to

beq $t0, $zero, y	# x = '\0'
beq $t1, $zero, x	# y = '\0'
bgt $t0, $t1, x		# x > y = 0
bgt $t1, $t0, y		# y > x = 1
beq $t0, $t1, equal	# x = y

lw $ra, 0($sp)		# Load $ra from stack
addi $sp, $sp, +4	# Contract stack to where it used to be
jr $ra			# Jump and return to $ra

equal:
addi $sp, $sp, -4	# Extend stack and store $ra
sw $ra, 0($sp)

addi $t2, $t2, 1	# $t2 = $t2 + 1
move $a2, $t2		# $a2 = $t2

jal str_lt

lw $ra, 0($sp)		# Load $ra from stack
addi $sp, $sp, +4	# Contract stack to where it used to be
jr $ra			# Jump and return to $ra

x:
move $v0, $zero		# $v0 = 0
jr $ra

y:
addi $v0, $zero, 1	# $v0 = 1
jr $ra

innerFor:
move $t0, $zero		# Reset everything
move $t1, $zero
move $t2, $zero
move $t3, $zero
move $t4, $zero
move $t5, $zero
move $t6, $zero
move $t7, $zero
move $t8, $zero

move $t4, $a0		# Value
move $t5, $a1		# i
addi $t6, $t5, -1	# j = i - 1
move $v0, $t6		# Return j--
move $s7, $t6		# Return j--

blt $t6, $s1, skip	# j < 1 go to end

mul $t7, $t6, 4 	# $t7 = j * 4
lw $t7, data($t7)	# Load address of data(j * 4) into $t7

move $a0, $t4		# $a0 = value
move $a1, $t7		# $a1 = data(j * 4)
move $a2, $zero		# $a2 = 0

## str_lt(value, a[j])
jal str_lt

move $t8, $v0
beq $t8, $zero, skip	# str_lt(value, a[j] = 0 then branch

addi $t8, $t6, +1	# $t8 = j + 1
mul $t8, $t8, 4		# $t8 = data(j * 4)
sw  $t7, data($t8)	# a[j+1] = a[j]

move $a0, $t4		# value
move $a1, $t6		# j--

jal innerFor		# Calls the innerFor

skip:

j outerForMiddle	# Jumps to outerForMiddle

outerFor:

bgt $t9, $s0, outerEnd	# i < length

mul $s5, $t9, 4		# $s5 = (i * 4)
lw $s5, data($s5)	# lw $s5 into $s5

move $a0, $s5		# Sets it up
move $a1, $t9
j innerFor		# Jumps to innerFor

outerForMiddle:

move $s6, $v0		# a[j+1] = value;
addi $s7, $s7, 1
mul $s6, $s7, 4
sw $s5, data($s6)

addi $t9, $t9, 1	# i ++

jal outerFor		# Calls outerFor

outerEnd:

j back			# Go back to main
