#include <ant_constants.h>
#define SRA_S 8
#define O_FP 4
#define O_GP 0
#define ABA_0 8
	.text
	.globl palette_size
	.align 2

	.ent palette_size

palette_size:
	subu	$sp, $sp, SRA_S
	sw		$fp, O_FP($sp)
	sw		$gp, O_GP($sp)
	move	$fp, $sp
	sw		$a0, ABA_0($fp)

	move	$t0, $0
	addu	$t1, $0, MAXCOLOR
	addiu	$t1, $t1, 1
$LOOP:
	addu	$t2, $t0, $a0
	lw		$t3, 0($t2)
	sub		$t4, $t3, $t1
	beqz	$t4, $FIN
	nop
	addiu	$t0, $t0, 4
	b		$LOOP
	nop 
$FIN:
	srl		$t0, $t0, 2
	move	$v0, $t0

	move	$sp, $fp
	lw		$fp, O_FP($sp)
	lw		$gp, O_GP($sp)
	addu	$sp, $sp, SRA_S
	jr		$ra

	.end	palette_size