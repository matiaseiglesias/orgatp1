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

#define ABA_PS_S 16

#define ABA_AA_S 16
	.text
	.globl paint_a
	.align 2

	.ent paint_a

paint_a:
	subu	$sp, $sp, SRA_S
	sw		$ra, O_RA($sp)
	sw		$fp, O_FP($sp)
	sw		$gp, O_GP($sp)
	move	$fp, $sp
	sw		$a0, ABA_0($fp)
	sw		$a1, ABA_1($fp)
	sw		$a2, ABA_2($fp)
	sw		$a3, ABA_3($fp)

	subu	$sp, $sp, LTA_S
	
	// store parameters in callee saved registers
	move	$s0, $a0
	move	$s1, $a1
	move	$s2, $a2
	move	$s3, $a3
	lw		$s4, ABA_4($fp)
	//call palette_size
	subu	$sp, $sp, ABA_PS_S
	move	$a0, $a2
	jal		palette_size
	addu	$sp, $sp, ABA_PS_S

	move	$t0, $v0
	sll		$t0, $t0, 2		// t0 <- t0 * 4
$LOOP_COLOUR:
	addu	$t3, $t0, $s3
	lw		$t1, 0($t3)		// t1 <- rules[i]
	addu	$t3, $t0, $s2
	lw		$t2, 0($t3)		// t2 <- palette[i]
	sll		$t2, $t2, 2		// t2 <- t2 * 4
	addu	$t2, $t2, $sp 
	sw		$t1, 0($t2)		// colour_rule[palette[i]] <- t1
	sub		$t0, $t0, 4
	bltz	$t0, $FIN_COLOUR
	nop
	b		$LOOP_COLOUR
	nop
$FIN_COLOUR:
	move	$s5, $0
	addu	$s5, $s5, 1			// i = 1
$LOOP_ITERATIONS:
	bne		$s5, $v0, $CONTINUE	// i != palette_len
	nop
	subu	$s4, $s4, $s5		// iterations = iterations - i;
	move	$s5, $0				// i = 0
$CONTINUE:
	lw		$t1, ANT_X($s0)		// t1 <- (ant->x)
	lw		$t2, ANT_Y($s0)		// t2 <- (ant->y)
	lw		$t3, GRID_WIDTH($s1)// t3 <- (grid->width)
	lw		$t4, GRID_GRID($s1)	// t4 <- (grid->grid)
	sll		$t5, $t2, 2			// t5 <- ant->y * 4
	addu	$t5, $t5, $t4		// t5 <- &(&(grid[x][ant->y])) (la columna)
	lw		$t5, 0($t5)			// t5 <- &(grid[x][ant->y]) (la columna)
	sll		$t1, $t1, 2			// t1 <- ant->x * 4
	addu	$t5, $t5, $t1		// t5 <- &(grid[ant->x][ant->y])
	lw 		$t6, 0($t5)			// t6 <- grid[ant->x][ant->y]
	sll		$t6, $t6, 2			// t6 <- grid[ant->x][ant->y] * 4
	addu	$t6, $t6, $sp		// t6 <- &(colour_rule[grid->grid[ant->x][ant->y]])
	lw		$t6, 0($t6)			// t6 <- colour_rule[grid->grid[ant->x][ant->y]]
	lw		$t7, ANT_O($s0)		// t7 <- (ant->o)
	mulou	$t6, $t6, 2			// t6 <- C_rot*y
	addu	$t6, $t6, $t7		// t6 <- C_rot*y + x
	mulou	$t6, $t6, 4			// t6 <- (x+C_rot*y)*sizeof(orientacion_t)
	la		$t7, rotaciones		// t7 <- &rotaciones
	addu	$t6, $t6, $t7		// t6 <- &(rotaciones[ant->o][colour_rule[grid->grid[ant->x][ant->y]]])
	lw		$t6, 0($t6)			// t6 <- rotaciones[ant->o][colour_rule[grid->grid[ant->x][ant->y]]]

	sll		$t1, $s5, 2
	addu	$t1, $t1, $s2		// t1 <- &(palette[i])
	lw		$t1, 0($t1)			// t1 <- palette[i]
	sw		$t1, 0($t5)			// grid->grid[ant->y][ant->x] = palette[i]

	sw		$t6, ANT_O($s0)		// ant->o = proxima_orientacion

	//call ant_advance
	subu	$sp, $sp, ABA_AA_S
	move	$a0, $s0
	move	$a1, $s1
	move	$a2, $t6
	jal		ant_advance
	addu	$sp, $sp, ABA_AA_S

	beq		$s5, $s4, $FIN_ITERATIONS
	nop
	addu	$s5, $s5, 1
	b		$LOOP_ITERATIONS
	nop
$FIN_ITERATIONS:
	move 	$v0, $s1 			// return grid

	move	$sp, $fp
	lw		$ra, O_RA($sp)
	lw		$fp, O_FP($sp)
	lw		$gp, O_GP($sp)
	addu	$sp, $sp, SRA_S
	jr		$ra
	
	.end	paint_a

.data
	rotaciones:
		.word OW
		.word OE
		.word ON
		.word OS
		.word OE
		.word OW
		.word OS
		.word ON
