#include <ant_constants.h>
#define SRA_S 16
#define O_RA 8
#define O_FP 4
#define O_GP 0
#define ABA_0 16
#define ABA_1 20
#define ABA_2 24
#define ABA_3 28
#define ABA_4 32

#define LTA_S 24
    .text
	.globl paint_a
	.align 2

	.ent paint_a

paint_a:
	subu 	$sp, $sp, SRA_S
	sw		$ra, O_RA($sp)
	sw		$fp, O_FP($sp)
	sw		$gp, O_GP($sp)
	move	$fp, $sp
	sw 		$a0, ABA_0($fp)
	sw 		$a1, ABA_1($fp)
	sw 		$a2, ABA_2($fp)
	sw 		$a3, ABA_3($fp)

	subu 	$sp, $sp, LTA_S
	addi 	$t0, $0, 2
	sll		$t0, $t0, 2   // t0 <- t0 * 4
$LOOP:
	addu  	$t3, $t0, $a3
	lw 		$t1, 0($t3)   // t1 <- rules[i]
	addu  	$t3, $t0, $a2
	lw		$t2, 0($t3)   // t2 <- palette[i]
	sll		$t2, $t2, 2   // t2 <- t2 * 4
	addu	$t2, $t2, $sp 
	sw		$t1, 0($t2)   // colour_rule[palette[i]] <- t1
	sub	$t0, $t0, 4
	bltz 	$t0, $FIN
	nop
	b 		$LOOP
	nop
$FIN:
	lw		$v0, CG($sp)

	move 	$sp, $fp
	lw		$ra, O_RA($sp)
	lw 		$fp, O_FP($sp)
	lw		$gp, O_GP($sp)
	addu 	$sp, $sp, SRA_S
	jr		$ra
	
	.end	paint_a

.data
	rotaciones: 
		.word OW
		.word OE
		.word OE
		.word OW
		.word ON
		.word OS
		.word OS
		.word ON
