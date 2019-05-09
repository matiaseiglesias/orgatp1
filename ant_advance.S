#include "ant_constants.h"
#define SRA_S 8
#define O_FP 4
#define O_GP 0
#define ABA_0 8
#define ABA_1 12
#define ABA_2 16
	.text
	.globl ant_advance
	.align 2

	.ent ant_advance
ant_advance:
	subu	$sp, $sp, SRA_S
	sw		$fp, O_FP($sp)
	sw		$gp, O_GP($sp)
	move	$fp, $sp
	sw		$a0, ABA_0($fp)
	sw		$a1, ABA_1($fp)
	sw		$a2, ABA_2($fp)

	lw		$t1, ANT_X($a0)		// t1 <- (ant->x)
	lw		$t2, ANT_Y($a0)		// t2 <- (ant->y)
	lw		$t3, GRID_WIDTH($a1)// t3 <- (grid->width)
	lw		$t4, GRID_HEIGHT($a1)// t4 <- (grid->height)

	la		$t0, movimientos 
	sll		$t5, $a2, 2
	addu	$t0, $t0, $t5	// t0 <- &(movimientos[orientacion])
	lw		$t0, 0($t0)

	move	$t5, $t2
	move	$t6, $t1
	jr		$t0
	nop
$NORTH:
	sub		$t5, $t2, 1
	bgez	$t5, $WRITE
	nop
	sub		$t5, $t4, 1
	b		$WRITE
	nop
$SOUTH:
	addu	$t5, $t2, 1
	bne		$t5, $t4, $WRITE
	nop
	move	$t5, $0
	b		$WRITE
	nop
$EAST:
	addu	$t6, $t1, 1
	bne		$t6, $t3, $WRITE
	nop
	move	$t6, $0
	b		$WRITE
	nop
$WEST:
	sub		$t6, $t1, 1
	bgez	$t6, $WRITE
	nop
	sub		$t6, $t3, 1
	b		$WRITE
	nop
$WRITE:
	sw		$t6, ANT_X($a0)
	sw		$t5, ANT_Y($a0)

	move	$sp, $fp
	lw		$fp, O_FP($sp)
	lw		$gp, O_GP($sp)
	addu	$sp, $sp, SRA_S
	jr		$ra

	.end	ant_advance

.data
	movimientos:
		.word $NORTH
		.word $SOUTH
		.word $EAST
		.word $WEST
