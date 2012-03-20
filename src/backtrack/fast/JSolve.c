/* JSolve.c - A very fast Sudoku solver

Version 1.1 of January 12, 2010

Copyright (c) 2009, Jason T. Linhart
All rights reserved.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
	¥	Redistributions of source code must retain the above copyright notice,
		this list of conditions and the following disclaimer.
	¥	Redistributions in binary form must reproduce the above copyright notice,
		this list of conditions and the following disclaimer in the documentation
		and/or other materials provided with the distribution.
	¥	Neither the name Jason T. Linhart nor the names of other contributors
		may be used to endorse or promote products derived from this software
		without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "JSolve.h"

#define UNROLL_LOOPS	1			// Set to 0 to disable manual loop unrolling

#define STACK_DEPTH		60
#define QUEUE_DEPTH		120

#define CONST						// Faster without const keyword on some compilers

#ifndef true						// Make sure these are defined
#define true			(-1)
#define false			0
#endif

#define DIGITS			9			// 9x9 Sudoku
#define NEIGHBORS		20			// Number of cells that can see this one
#define HOUSES			27			// Houses (rows, then columns, then blocks)
#define CELLS			81			// Cells
#define NoDigit			0xFF
#define NoCell			0xFF
#define MaskAllDigits	0x1FF		// one bit per digit, all bits set
#define MASKS			512			// 2^DIGITS

typedef int DigitMask;				// Digit possibilities
typedef int Boolean;				// true or false
typedef unsigned char Digit;		// Digits (0 to 8)
typedef unsigned char House;		// House (0-26, rows, then cols, then blocks)
typedef unsigned char Cell;			// Cells (0 to 80)

#define RowH(cell)					row_house[cell]
#define ColH(cell)					col_house[cell]
#define BlockH(cell)				block_house[cell]
#define HouseCellList(house,indx)	t_cell[house][indx]
#define CountOnes(mask)				t_mask_count_ones[mask]
#define DigitToMask(digit)			(1<<(digit))
#define MaskToDigit(mask)			t_mask_digit[mask]
#define NeighborList(cell)			t_neighbors[cell]

static int solution_count;						// Number of solutions found so far
static int cells_remaining;						// How many cells need to be filled in
static int invalid_board;						// Can't be solved when true
static int solution[CELLS];						// The solution as mask bits
static DigitMask board[CELLS];					// Mask bits for digits allowed in cell
static int house_solved[HOUSES];				// Mask bits for digits solved in house

static DigitMask board_stack[STACK_DEPTH][CELLS];// Undo stack of 'board' for guesses
static int solved_stack[STACK_DEPTH][HOUSES];	// Undo stack of 'house_solved' for guesses
static int stack_depth;							// How many items on the stack

static int queue_cell[QUEUE_DEPTH];				// Queue of digits to set: cell
static int queue_mask[QUEUE_DEPTH];				// Mask bit for digit to set
static int queue_depth;							// Number of cell/digit pairs in the queue

static int guess_cell[STACK_DEPTH];				// Which cell we guessed at
static int guess_mask[STACK_DEPTH];				// Remaining possibilites if guess is wrong
static int guess_remaining[STACK_DEPTH];		// Number of cells remaining to be solved

static CONST House row_house[CELLS] = {			// House # of cell's row
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 8, 8 };
static CONST House col_house[CELLS] = {			// House # of cell's col
	9, 10, 11, 12, 13, 14, 15, 16, 17,
	9, 10, 11, 12, 13, 14, 15, 16, 17,
	9, 10, 11, 12, 13, 14, 15, 16, 17,
	9, 10, 11, 12, 13, 14, 15, 16, 17,
	9, 10, 11, 12, 13, 14, 15, 16, 17,
	9, 10, 11, 12, 13, 14, 15, 16, 17,
	9, 10, 11, 12, 13, 14, 15, 16, 17,
	9, 10, 11, 12, 13, 14, 15, 16, 17,
	9, 10, 11, 12, 13, 14, 15, 16, 17 };
static CONST House block_house[CELLS] = {		// House # of cell's block
	18, 18, 18, 19, 19, 19, 20, 20, 20,
	18, 18, 18, 19, 19, 19, 20, 20, 20,
	18, 18, 18, 19, 19, 19, 20, 20, 20,
	21, 21, 21, 22, 22, 22, 23, 23, 23,
	21, 21, 21, 22, 22, 22, 23, 23, 23,
	21, 21, 21, 22, 22, 22, 23, 23, 23,
	24, 24, 24, 25, 25, 25, 26, 26, 26,
	24, 24, 24, 25, 25, 25, 26, 26, 26,
	24, 24, 24, 25, 25, 25, 26, 26, 26 };
static CONST Cell t_cell[HOUSES][DIGITS] = {	// Cell number of each cell in the house
	{	 0,  1,  2,  3,  4,  5,  6,  7,  8	},		// Rows
	{	 9, 10, 11, 12, 13, 14, 15, 16, 17	},
	{	18, 19, 20, 21, 22, 23, 24, 25, 26	},
	{	27, 28, 29, 30, 31, 32, 33, 34, 35	},
	{	36, 37, 38, 39, 40, 41, 42, 43, 44	},
	{	45, 46, 47, 48, 49, 50, 51, 52, 53	},
	{	54, 55, 56, 57, 58, 59, 60, 61, 62	},
	{	63, 64, 65, 66, 67, 68, 69, 70, 71	},
	{	72, 73, 74, 75, 76, 77, 78, 79, 80	},
	{	 0,  9, 18, 27, 36, 45, 54, 63, 72	},		// Cols
	{	 1, 10, 19, 28, 37, 46, 55, 64, 73	},
	{	 2, 11, 20, 29, 38, 47, 56, 65, 74	},
	{	 3, 12, 21, 30, 39, 48, 57, 66, 75	},
	{	 4, 13, 22, 31, 40, 49, 58, 67, 76	},
	{	 5, 14, 23, 32, 41, 50, 59, 68, 77	},
	{	 6, 15, 24, 33, 42, 51, 60, 69, 78	},
	{	 7, 16, 25, 34, 43, 52, 61, 70, 79	},
	{	 8, 17, 26, 35, 44, 53, 62, 71, 80	},
	{	 0,  1,  2,  9, 10, 11, 18, 19, 20	},		// Blocks
	{	 3,  4,  5, 12, 13, 14, 21, 22, 23	},
	{	 6,  7,  8, 15, 16, 17, 24, 25, 26	},
	{	27, 28, 29, 36, 37, 38, 45, 46, 47	},
	{	30, 31, 32, 39, 40, 41, 48, 49, 50	},
	{	33, 34, 35, 42, 43, 44, 51, 52, 53	},
	{	54, 55, 56, 63, 64, 65, 72, 73, 74	},
	{	57, 58, 59, 66, 67, 68, 75, 76, 77	},
	{	60, 61, 62, 69, 70, 71, 78, 79, 80	}
	};
static CONST Cell t_mask_count_ones[MASKS] = {	// Number of bits set in the mask
	0,	1,	1,	2,	1,	2,	2,	3,	1,	2,	2,	3,	2,	3,	3,	4,
	1,	2,	2,	3,	2,	3,	3,	4,	2,	3,	3,	4,	3,	4,	4,	5,
	1,	2,	2,	3,	2,	3,	3,	4,	2,	3,	3,	4,	3,	4,	4,	5,
	2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
	1,	2,	2,	3,	2,	3,	3,	4,	2,	3,	3,	4,	3,	4,	4,	5,
	2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
	2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
	3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
	1,	2,	2,	3,	2,	3,	3,	4,	2,	3,	3,	4,	3,	4,	4,	5,
	2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
	2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
	3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
	2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
	3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
	3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
	4,	5,	5,	6,	5,	6,	6,	7,	5,	6,	6,	7,	6,	7,	7,	8,
	1,	2,	2,	3,	2,	3,	3,	4,	2,	3,	3,	4,	3,	4,	4,	5,
	2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
	2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
	3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
	2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
	3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
	3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
	4,	5,	5,	6,	5,	6,	6,	7,	5,	6,	6,	7,	6,	7,	7,	8,
	2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
	3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
	3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
	4,	5,	5,	6,	5,	6,	6,	7,	5,	6,	6,	7,	6,	7,	7,	8,
	3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
	4,	5,	5,	6,	5,	6,	6,	7,	5,	6,	6,	7,	6,	7,	7,	8,
	4,	5,	5,	6,	5,	6,	6,	7,	5,	6,	6,	7,	6,	7,	7,	8,
	5,	6,	6,	7,	6,	7,	7,	8,	6,	7,	7,	8,	7,	8,	8,	9
	};
#define ND	NoDigit
static CONST Digit t_mask_digit[MASKS] = {		// Digit set in the mask (if only one)
	ND,	0,	1,	ND,	2,	ND,	ND,	ND,	3,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	4,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	5,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	6,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	7,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	8,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,
	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND,	ND
	};
												// Neighbors of each cell
static CONST Cell t_neighbors[CELLS][NEIGHBORS] = {
	{ 9, 18, 27, 36, 45, 54, 63, 72, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 19, 20 },
	{ 10, 19, 28, 37, 46, 55, 64, 73, 0, 2, 3, 4, 5, 6, 7, 8, 9, 11, 18, 20 },
	{ 11, 20, 29, 38, 47, 56, 65, 74, 0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 18, 19 },
	{ 12, 21, 30, 39, 48, 57, 66, 75, 0, 1, 2, 4, 5, 6, 7, 8, 13, 14, 22, 23 },
	{ 13, 22, 31, 40, 49, 58, 67, 76, 0, 1, 2, 3, 5, 6, 7, 8, 12, 14, 21, 23 },
	{ 14, 23, 32, 41, 50, 59, 68, 77, 0, 1, 2, 3, 4, 6, 7, 8, 12, 13, 21, 22 },
	{ 15, 24, 33, 42, 51, 60, 69, 78, 0, 1, 2, 3, 4, 5, 7, 8, 16, 17, 25, 26 },
	{ 16, 25, 34, 43, 52, 61, 70, 79, 0, 1, 2, 3, 4, 5, 6, 8, 15, 17, 24, 26 },
	{ 17, 26, 35, 44, 53, 62, 71, 80, 0, 1, 2, 3, 4, 5, 6, 7, 15, 16, 24, 25 },
	{ 0, 18, 27, 36, 45, 54, 63, 72, 10, 11, 12, 13, 14, 15, 16, 17, 1, 2, 19, 20 },
	{ 1, 19, 28, 37, 46, 55, 64, 73, 9, 11, 12, 13, 14, 15, 16, 17, 0, 2, 18, 20 },
	{ 2, 20, 29, 38, 47, 56, 65, 74, 9, 10, 12, 13, 14, 15, 16, 17, 0, 1, 18, 19 },
	{ 3, 21, 30, 39, 48, 57, 66, 75, 9, 10, 11, 13, 14, 15, 16, 17, 4, 5, 22, 23 },
	{ 4, 22, 31, 40, 49, 58, 67, 76, 9, 10, 11, 12, 14, 15, 16, 17, 3, 5, 21, 23 },
	{ 5, 23, 32, 41, 50, 59, 68, 77, 9, 10, 11, 12, 13, 15, 16, 17, 3, 4, 21, 22 },
	{ 6, 24, 33, 42, 51, 60, 69, 78, 9, 10, 11, 12, 13, 14, 16, 17, 7, 8, 25, 26 },
	{ 7, 25, 34, 43, 52, 61, 70, 79, 9, 10, 11, 12, 13, 14, 15, 17, 6, 8, 24, 26 },
	{ 8, 26, 35, 44, 53, 62, 71, 80, 9, 10, 11, 12, 13, 14, 15, 16, 6, 7, 24, 25 },
	{ 0, 9, 27, 36, 45, 54, 63, 72, 19, 20, 21, 22, 23, 24, 25, 26, 1, 2, 10, 11 },
	{ 1, 10, 28, 37, 46, 55, 64, 73, 18, 20, 21, 22, 23, 24, 25, 26, 0, 2, 9, 11 },
	{ 2, 11, 29, 38, 47, 56, 65, 74, 18, 19, 21, 22, 23, 24, 25, 26, 0, 1, 9, 10 },
	{ 3, 12, 30, 39, 48, 57, 66, 75, 18, 19, 20, 22, 23, 24, 25, 26, 4, 5, 13, 14 },
	{ 4, 13, 31, 40, 49, 58, 67, 76, 18, 19, 20, 21, 23, 24, 25, 26, 3, 5, 12, 14 },
	{ 5, 14, 32, 41, 50, 59, 68, 77, 18, 19, 20, 21, 22, 24, 25, 26, 3, 4, 12, 13 },
	{ 6, 15, 33, 42, 51, 60, 69, 78, 18, 19, 20, 21, 22, 23, 25, 26, 7, 8, 16, 17 },
	{ 7, 16, 34, 43, 52, 61, 70, 79, 18, 19, 20, 21, 22, 23, 24, 26, 6, 8, 15, 17 },
	{ 8, 17, 35, 44, 53, 62, 71, 80, 18, 19, 20, 21, 22, 23, 24, 25, 6, 7, 15, 16 },
	{ 0, 9, 18, 36, 45, 54, 63, 72, 28, 29, 30, 31, 32, 33, 34, 35, 37, 38, 46, 47 },
	{ 1, 10, 19, 37, 46, 55, 64, 73, 27, 29, 30, 31, 32, 33, 34, 35, 36, 38, 45, 47 },
	{ 2, 11, 20, 38, 47, 56, 65, 74, 27, 28, 30, 31, 32, 33, 34, 35, 36, 37, 45, 46 },
	{ 3, 12, 21, 39, 48, 57, 66, 75, 27, 28, 29, 31, 32, 33, 34, 35, 40, 41, 49, 50 },
	{ 4, 13, 22, 40, 49, 58, 67, 76, 27, 28, 29, 30, 32, 33, 34, 35, 39, 41, 48, 50 },
	{ 5, 14, 23, 41, 50, 59, 68, 77, 27, 28, 29, 30, 31, 33, 34, 35, 39, 40, 48, 49 },
	{ 6, 15, 24, 42, 51, 60, 69, 78, 27, 28, 29, 30, 31, 32, 34, 35, 43, 44, 52, 53 },
	{ 7, 16, 25, 43, 52, 61, 70, 79, 27, 28, 29, 30, 31, 32, 33, 35, 42, 44, 51, 53 },
	{ 8, 17, 26, 44, 53, 62, 71, 80, 27, 28, 29, 30, 31, 32, 33, 34, 42, 43, 51, 52 },
	{ 0, 9, 18, 27, 45, 54, 63, 72, 37, 38, 39, 40, 41, 42, 43, 44, 28, 29, 46, 47 },
	{ 1, 10, 19, 28, 46, 55, 64, 73, 36, 38, 39, 40, 41, 42, 43, 44, 27, 29, 45, 47 },
	{ 2, 11, 20, 29, 47, 56, 65, 74, 36, 37, 39, 40, 41, 42, 43, 44, 27, 28, 45, 46 },
	{ 3, 12, 21, 30, 48, 57, 66, 75, 36, 37, 38, 40, 41, 42, 43, 44, 31, 32, 49, 50 },
	{ 4, 13, 22, 31, 49, 58, 67, 76, 36, 37, 38, 39, 41, 42, 43, 44, 30, 32, 48, 50 },
	{ 5, 14, 23, 32, 50, 59, 68, 77, 36, 37, 38, 39, 40, 42, 43, 44, 30, 31, 48, 49 },
	{ 6, 15, 24, 33, 51, 60, 69, 78, 36, 37, 38, 39, 40, 41, 43, 44, 34, 35, 52, 53 },
	{ 7, 16, 25, 34, 52, 61, 70, 79, 36, 37, 38, 39, 40, 41, 42, 44, 33, 35, 51, 53 },
	{ 8, 17, 26, 35, 53, 62, 71, 80, 36, 37, 38, 39, 40, 41, 42, 43, 33, 34, 51, 52 },
	{ 0, 9, 18, 27, 36, 54, 63, 72, 46, 47, 48, 49, 50, 51, 52, 53, 28, 29, 37, 38 },
	{ 1, 10, 19, 28, 37, 55, 64, 73, 45, 47, 48, 49, 50, 51, 52, 53, 27, 29, 36, 38 },
	{ 2, 11, 20, 29, 38, 56, 65, 74, 45, 46, 48, 49, 50, 51, 52, 53, 27, 28, 36, 37 },
	{ 3, 12, 21, 30, 39, 57, 66, 75, 45, 46, 47, 49, 50, 51, 52, 53, 31, 32, 40, 41 },
	{ 4, 13, 22, 31, 40, 58, 67, 76, 45, 46, 47, 48, 50, 51, 52, 53, 30, 32, 39, 41 },
	{ 5, 14, 23, 32, 41, 59, 68, 77, 45, 46, 47, 48, 49, 51, 52, 53, 30, 31, 39, 40 },
	{ 6, 15, 24, 33, 42, 60, 69, 78, 45, 46, 47, 48, 49, 50, 52, 53, 34, 35, 43, 44 },
	{ 7, 16, 25, 34, 43, 61, 70, 79, 45, 46, 47, 48, 49, 50, 51, 53, 33, 35, 42, 44 },
	{ 8, 17, 26, 35, 44, 62, 71, 80, 45, 46, 47, 48, 49, 50, 51, 52, 33, 34, 42, 43 },
	{ 0, 9, 18, 27, 36, 45, 63, 72, 55, 56, 57, 58, 59, 60, 61, 62, 64, 65, 73, 74 },
	{ 1, 10, 19, 28, 37, 46, 64, 73, 54, 56, 57, 58, 59, 60, 61, 62, 63, 65, 72, 74 },
	{ 2, 11, 20, 29, 38, 47, 65, 74, 54, 55, 57, 58, 59, 60, 61, 62, 63, 64, 72, 73 },
	{ 3, 12, 21, 30, 39, 48, 66, 75, 54, 55, 56, 58, 59, 60, 61, 62, 67, 68, 76, 77 },
	{ 4, 13, 22, 31, 40, 49, 67, 76, 54, 55, 56, 57, 59, 60, 61, 62, 66, 68, 75, 77 },
	{ 5, 14, 23, 32, 41, 50, 68, 77, 54, 55, 56, 57, 58, 60, 61, 62, 66, 67, 75, 76 },
	{ 6, 15, 24, 33, 42, 51, 69, 78, 54, 55, 56, 57, 58, 59, 61, 62, 70, 71, 79, 80 },
	{ 7, 16, 25, 34, 43, 52, 70, 79, 54, 55, 56, 57, 58, 59, 60, 62, 69, 71, 78, 80 },
	{ 8, 17, 26, 35, 44, 53, 71, 80, 54, 55, 56, 57, 58, 59, 60, 61, 69, 70, 78, 79 },
	{ 0, 9, 18, 27, 36, 45, 54, 72, 64, 65, 66, 67, 68, 69, 70, 71, 55, 56, 73, 74 },
	{ 1, 10, 19, 28, 37, 46, 55, 73, 63, 65, 66, 67, 68, 69, 70, 71, 54, 56, 72, 74 },
	{ 2, 11, 20, 29, 38, 47, 56, 74, 63, 64, 66, 67, 68, 69, 70, 71, 54, 55, 72, 73 },
	{ 3, 12, 21, 30, 39, 48, 57, 75, 63, 64, 65, 67, 68, 69, 70, 71, 58, 59, 76, 77 },
	{ 4, 13, 22, 31, 40, 49, 58, 76, 63, 64, 65, 66, 68, 69, 70, 71, 57, 59, 75, 77 },
	{ 5, 14, 23, 32, 41, 50, 59, 77, 63, 64, 65, 66, 67, 69, 70, 71, 57, 58, 75, 76 },
	{ 6, 15, 24, 33, 42, 51, 60, 78, 63, 64, 65, 66, 67, 68, 70, 71, 61, 62, 79, 80 },
	{ 7, 16, 25, 34, 43, 52, 61, 79, 63, 64, 65, 66, 67, 68, 69, 71, 60, 62, 78, 80 },
	{ 8, 17, 26, 35, 44, 53, 62, 80, 63, 64, 65, 66, 67, 68, 69, 70, 60, 61, 78, 79 },
	{ 0, 9, 18, 27, 36, 45, 54, 63, 73, 74, 75, 76, 77, 78, 79, 80, 55, 56, 64, 65 },
	{ 1, 10, 19, 28, 37, 46, 55, 64, 72, 74, 75, 76, 77, 78, 79, 80, 54, 56, 63, 65 },
	{ 2, 11, 20, 29, 38, 47, 56, 65, 72, 73, 75, 76, 77, 78, 79, 80, 54, 55, 63, 64 },
	{ 3, 12, 21, 30, 39, 48, 57, 66, 72, 73, 74, 76, 77, 78, 79, 80, 58, 59, 67, 68 },
	{ 4, 13, 22, 31, 40, 49, 58, 67, 72, 73, 74, 75, 77, 78, 79, 80, 57, 59, 66, 68 },
	{ 5, 14, 23, 32, 41, 50, 59, 68, 72, 73, 74, 75, 76, 78, 79, 80, 57, 58, 66, 67 },
	{ 6, 15, 24, 33, 42, 51, 60, 69, 72, 73, 74, 75, 76, 77, 79, 80, 61, 62, 70, 71 },
	{ 7, 16, 25, 34, 43, 52, 61, 70, 72, 73, 74, 75, 76, 77, 78, 80, 60, 62, 69, 71 },
	{ 8, 17, 26, 35, 44, 53, 62, 71, 72, 73, 74, 75, 76, 77, 78, 79, 60, 61, 69, 70 }
	};

static void
Queue(int cell,int mask)			// Queue a digit (as a mask) to be set in the given cell
{
	queue_cell[queue_depth]=(cell);
	queue_mask[queue_depth]=(mask);
	++queue_depth;
	}

static void
ProcessQueue(void)					// Process all items in the queue
{
	int indx, cnt, cell, ncell, mask, ones;
	CONST Cell *nptr;

	if (queue_depth<4) {								// < 4 items get done one at a time
		for (indx=0; indx<queue_depth; ++indx) {		// For each item in the queue
			cell=queue_cell[indx];
			if (board[cell]) {							// If not already done
				mask=queue_mask[indx];
				if (!(board[cell]&mask)) {				// If not still possible, problem
					invalid_board=true;
					return;
					}
				board[cell]=0;							// Mark the cell as solved
				solution[cell]=mask;					// Store the solution for later
				house_solved[RowH(cell)]|=mask;			// Mark solved in the houses
				house_solved[ColH(cell)]|=mask;
				house_solved[BlockH(cell)]|=mask;
				--cells_remaining;						// One less to be done
				nptr=NeighborList(cell);
#if UNROLL_LOOPS
				for (cnt=0; cnt<NEIGHBORS; cnt+=4) {	// For every neighbor
					ncell=nptr[0];
					if (board[ncell]&mask) {			// Clear the bit
						ones=CountOnes((board[ncell]^=mask));
						if (ones==1) Queue(ncell,board[ncell]);
						else if (ones<=0) {				// Nothing left, problem
							invalid_board=true;
							return;
							}
						}
					ncell=nptr[1];
					if (board[ncell]&mask) {			// Clear the bit
						ones=CountOnes((board[ncell]^=mask));
						if (ones==1) Queue(ncell,board[ncell]);
						else if (ones<=0) {				// Nothing left, problem
							invalid_board=true;
							return;
							}
						}
					ncell=nptr[2];
					if (board[ncell]&mask) {			// Clear the bit
						ones=CountOnes((board[ncell]^=mask));
						if (ones==1) Queue(ncell,board[ncell]);
						else if (ones<=0) {				// Nothing left, problem
							invalid_board=true;
							return;
							}
						}
					ncell=nptr[3];
					if (board[ncell]&mask) {			// Clear the bit
						ones=CountOnes((board[ncell]^=mask));
						if (ones==1) Queue(ncell,board[ncell]);
						else if (ones<=0) {				// Nothing left, problem
							invalid_board=true;
							return;
							}
						}
					nptr+=4;
					}
#else
				for (cnt=0; cnt<NEIGHBORS; ++cnt) {		// For every neighbor
					ncell = *nptr++;
					if (board[ncell]&mask) {			// Clear the bit
						ones=CountOnes((board[ncell]^=mask));
						if (ones==1) Queue(ncell,board[ncell]);
						else if (ones<=0) {				// Nothing left, problem
							invalid_board=true;
							return;
							}
						}
					}
#endif
				}
			}
		assert(queue_depth<QUEUE_DEPTH);
		queue_depth=0;
		}
	else {												// Many items, scan the entire
		for (indx=0; indx<queue_depth; ++indx) {		// board at the end
			cell=queue_cell[indx];
			if (board[cell]) {							// If not already done
				mask=queue_mask[indx];
				if (!(board[cell]&mask) || 				// If not still possible, problem
					 (house_solved[RowH(cell)]&mask) || 
					 (house_solved[ColH(cell)]&mask) || 
					 (house_solved[BlockH(cell)]&mask)) {
					invalid_board=true;
					return;
					}
				board[cell]=0;							// Mark the cell as solved
				solution[cell]=mask;					// Store the solution for later
				house_solved[RowH(cell)]|=mask;			// Mark solved in the houses
				house_solved[ColH(cell)]|=mask;
				house_solved[BlockH(cell)]|=mask;
				--cells_remaining;						// One less to be done
				}
			}
		assert(queue_depth<QUEUE_DEPTH);
		queue_depth=0;
		for (cell=0; cell<CELLS; ++cell) {				// Now scan the entire board,
														// clearing eliminated possibilities
			mask=house_solved[RowH(cell)]|house_solved[ColH(cell)]|
				house_solved[BlockH(cell)];
			if (board[cell]&mask) {						// If extra possibilities in the
				board[cell] &= ~mask;					// cell, clear them
				ones=CountOnes(board[cell]);
				if (ones==1) Queue(cell,board[cell]);	// One left is naked single
				else if (ones<=0) {						// Nothing left is a problem
					invalid_board=true;
					return;
					}
				}
			}
		}
	}

static void
HiddenSingles(void)					// Look for hidden singles
{
	int h, d, cell, mask;
	int at_least_once, more_than_once, once;

	for (h=0; h<HOUSES; ++h) {						// Check all houses
		at_least_once=more_than_once=0;				// Find possibilities that only
#if UNROLL_LOOPS
		for (d=0; d<DIGITS; d+=3) {					// occur in one cell in the house
			mask=board[HouseCellList(h,d)];
			more_than_once |= at_least_once&mask;
			at_least_once |= mask;
	
			mask=board[HouseCellList(h,d+1)];
			more_than_once |= at_least_once&mask;
			at_least_once |= mask;
	
			mask=board[HouseCellList(h,d+2)];
			more_than_once |= at_least_once&mask;
			at_least_once |= mask;
			}
#else
		for (d=0; d<DIGITS; ++d) {					// occur in one cell in the house
			mask=board[HouseCellList(h,d)];
			more_than_once |= at_least_once&mask;
			at_least_once |= mask;
			}
#endif
		if ((at_least_once|house_solved[h])!=MaskAllDigits) {
			invalid_board=true;						// Already solved plus still possible
			break;									// should include all digits
			}
		once=at_least_once & ~more_than_once;
		if (once) {
			for (d=0; d<DIGITS; ++d) {				// We know which digits are singles
				cell=HouseCellList(h,d);			// now we need to find one in the
				if ((mask=board[cell]&once)) {		// cells of the house
					if (CountOnes(mask)>1) {
						invalid_board=true;			// Two digits can't be single in
						return;						// one cell
						}
					Queue(cell,mask);				// Got it, queue it
					if (!(once^=mask)) return;
					}
				}
			return;									// Should never get to here
			}
		}
	}

/*
	LockedCandidates

	+----------+----------+----------+
	| A1 A2 A3 | B1 B2 B3 | C1 C2 C3 |
	| D1 D2 D3 | E1 E2 E3 | F1 F2 F3 |
	| G1 G2 G3 | G1 G2 G3 | H1 H2 H3 |
	+----------+----------+----------+

	Take a chute (three blocks in a line), named by the lowest numbered house that it
	contains, and divide it up into 9 strips, each with 3 cells in the intersection of a
	line and a block.
	
	Each strip has four neighboring strips, which share either a line or a block with it.
	These divide into two pairs of two strips, each pair making up the strips that share a
	single house with the original strip. For example, strip A in the diagram above shares
	a block with strips D and G, and shares a line with strips B and C.

	If one of the pairs of neighboring strips (BC or DG) is missing a digit, which is
	contained in the original strip, we have a locked candidate. The candidate is locked
	to the original strip and can not appear in any of the four neighbors. If the other
	paid of neighboring strips contains that digit, we have an elimination.

	This routine is based on looking for candidates that occur in the primary strip and
	one pair of neighboring strips, but not int the other pair of neighboring strips. When
	that happens there is an elimination to be made. This can easily be calculated for all
	digits at once using bitwise operations on words.
*/

static CONST unsigned char strip_map[DIGITS][4] = {
	{ 1, 2, 3, 6 },
	{ 0, 2, 4, 7 },
	{ 0, 1, 5, 8 },
	{ 4, 5, 0, 6 },
	{ 3, 5, 1, 7 },
	{ 3, 4, 2, 8 },
	{ 7, 8, 0, 3 },
	{ 6, 8, 1, 4 },
	{ 6, 7, 2, 5 }
	};

static void
LockedCandidates(void)				// Look for locked candidates
{
	int chute, h, strip, mask, cell, cnt;
	int a, b, c;
	int strips[9];
	unsigned char *cptr;

	for (chute=0; chute<6; ++chute) {					// for each of the six chutes
		h=chute*3;										// make a possibility mask for each strip
		strips[0]=board[HouseCellList(h,0)] | board[HouseCellList(h,1)] | board[HouseCellList(h,2)];
		strips[1]=board[HouseCellList(h,3)] | board[HouseCellList(h,4)] | board[HouseCellList(h,5)];
		strips[2]=board[HouseCellList(h,6)] | board[HouseCellList(h,7)] | board[HouseCellList(h,8)];
		strips[3]=board[HouseCellList(h+1,0)] | board[HouseCellList(h+1,1)] | board[HouseCellList(h+1,2)];
		strips[4]=board[HouseCellList(h+1,3)] | board[HouseCellList(h+1,4)] | board[HouseCellList(h+1,5)];
		strips[5]=board[HouseCellList(h+1,6)] | board[HouseCellList(h+1,7)] | board[HouseCellList(h+1,8)];
		strips[6]=board[HouseCellList(h+2,0)] | board[HouseCellList(h+2,1)] | board[HouseCellList(h+2,2)];
		strips[7]=board[HouseCellList(h+2,3)] | board[HouseCellList(h+2,4)] | board[HouseCellList(h+2,5)];
		strips[8]=board[HouseCellList(h+2,6)] | board[HouseCellList(h+2,7)] | board[HouseCellList(h+2,8)];
		for (strip=0; strip<DIGITS; ++strip) {			// for each of the nine strips
			cptr=strip_map[strip];						// get list of strips visible from this one
														// Anything in the main strip and one pair
														// but not the other is a locked candidate
			mask=(strips[strip] & ((strips[cptr[0]]|strips[cptr[1]])^(strips[cptr[2]]|strips[cptr[3]])));
			if (mask) {									// If we found a locked candidate
				for (a=0; a<4; ++a) {					// For each of the neighboring strips
					b=cptr[a];
#if 0													// Adding this slows things down
					if (!strips[b]) continue;			// Is there anything to do in this strip?
#endif
														// Get the house # (h=chute*3+b/3)
					h=chute*3+"\x00\x00\x00\x01\x01\x01\x02\x02\x02"[b];
														// and the base cell offset (c=(b%3)*3)
					c="\x00\x03\x06\x00\x03\x06\x00\x03\x06"[b];
#if UNROLL_LOOPS
					cell=HouseCellList(h,c);
					if (board[cell]&mask) {				// if any masked bits are set
						board[cell] &= ~mask;			// clear them
						cnt=CountOnes(board[cell]);
						if (cnt==0) {					// no possibilities left - invalid board
							invalid_board=true;
							return;
							}							// only one possibility? queue it
						else if (cnt==1) Queue(cell,board[cell]);
						}
					cell=HouseCellList(h,c+1);
					if (board[cell]&mask) {				// if any masked bits are set
						board[cell] &= ~mask;			// clear them
						cnt=CountOnes(board[cell]);
						if (cnt==0) {					// no possibilities left - invalid board
							invalid_board=true;
							return;
							}							// only one possibility? queue it
						else if (cnt==1) Queue(cell,board[cell]);
						}
					cell=HouseCellList(h,c+2);
					if (board[cell]&mask) {				// if any masked bits are set
						board[cell] &= ~mask;			// clear them
						cnt=CountOnes(board[cell]);
						if (cnt==0) {					// no possibilities left - invalid board
							invalid_board=true;
							return;
							}							// only one possibility? queue it
						else if (cnt==1) Queue(cell,board[cell]);
						}
#else
					for (d=0; d<3; ++d) {				// For each cell in current strip
						cell=HouseCellList(h,c+d);
						if (board[cell]&mask) {			// if any masked bits are set
							board[cell] &= ~mask;		// clear them
							cnt=CountOnes(board[cell]);
							if (cnt==0) {				// no possibilities left - invalid board
								invalid_board=true;
								return;
								}						// only one possibility? queue it
							else if (cnt==1) Queue(cell,board[cell]);
							}
						}
#endif
					}
				return;									// We found something, so abandon the search
				}
			}
		}
	}

static void
Guess(void)							// Guess at a digit
{
	int cell, best, bits, best_bits, mask;
	static int last_cell = 0;

	best=NoCell;										// Search all cells for the one
	best_bits=DIGITS+1;									// with the fewest possibilities
	cell=last_cell;										// Fewer possibilities means
	do {												// rapid elimination of search
		bits=CountOnes(board[cell]);					// "space"
		if (bits>1 && bits<best_bits) {
			best=cell;
			if (bits==2) break;							// 2 is ideal, abort search
			best_bits=bits;
			}
		if (++cell>=CELLS) cell=0;
		} while (cell!=last_cell);
	last_cell=cell;
	if (best==NoCell) invalid_board=true;				// Really shouldn't happen
	else {
		assert(stack_depth<STACK_DEPTH);				// Push current state on the stack
		guess_cell[stack_depth]=best;
		mask=board[best];
		mask &= -mask;									// Finds the lowest set bit
		guess_mask[stack_depth]=board[best]^mask;		// which is used as the guess
		guess_remaining[stack_depth]=cells_remaining;
		memcpy(board_stack[stack_depth],board,CELLS*sizeof(DigitMask));
		memcpy(solved_stack[stack_depth],house_solved,HOUSES*sizeof(int));
		++stack_depth;
		Queue(best,mask);								// And queue the guess
		}
	}

static void
PopStack(void)						// Undo a guess by going back to the stacked state
{
	int cell, mask;

	--stack_depth;										// Pop an element off the stack
	queue_depth=0;
	if (stack_depth>=0) {								// If not over, restore the state
		memcpy(board,board_stack[stack_depth],CELLS*sizeof(DigitMask));
		memcpy(house_solved,solved_stack[stack_depth],HOUSES*sizeof(int));
		cells_remaining=guess_remaining[stack_depth];
		cell=guess_cell[stack_depth];
		mask=guess_mask[stack_depth];
		if (CountOnes(mask)>1) board[cell]=mask;		// The cell we guessed in now has
		else Queue(cell,mask);							// one fewer option, may set digit
		invalid_board=false;
		}
	}

int									// Calculate number of solutions
JSolve(const char *clues,char *result,int max_solutions)
{									// 'clues' an 81 char string with digit or '.' per cell
									// 'result' if not null, write solution here
									// 'max_solutions' stop when reach this #
	int cell;

	memset(house_solved,0,sizeof(house_solved));		// or solved
	queue_depth=0;										// Nothing in the queue
#if UNROLL_LOOPS
	for (cell=0; cell<CELLS; cell+=3) {					// Clear the board
		board[cell]=MaskAllDigits;						// and queue clues
		if (isdigit(*clues) && *clues!='0') Queue(cell,DigitToMask(*clues-'1'));
		board[cell+1]=MaskAllDigits;					// and queue clues
		if (isdigit(clues[1]) && clues[1]!='0') Queue(cell+1,DigitToMask(clues[1]-'1'));
		board[cell+2]=MaskAllDigits;					// and queue clues
		if (isdigit(clues[2]) && clues[2]!='0') Queue(cell+2,DigitToMask(clues[2]-'1'));
		clues+=3;
		}
#else
	for (cell=0; cell<CELLS; ++cell) {					// Clear the board
		board[cell]=MaskAllDigits;						// and queue clues
		if (isdigit(*clues) && *clues!='0') Queue(cell,DigitToMask(*clues-'1'));
		++clues;
		}
#endif
	stack_depth=0;										// Nothing on the stack
	solution_count=0;									// Nothing solved (yet)
	cells_remaining=CELLS;								// with lots of work to do
	invalid_board=false;								// Board starts valid
	while (stack_depth>=0) {							// Keep going till total failure
		if (queue_depth>0) ProcessQueue();				// Process everything on the queue
		if (cells_remaining>0 && !invalid_board) {		// if not finished
			HiddenSingles();							// try hidden singles
			if (queue_depth<=0 && !invalid_board) {		// if that didn't do anything, try locked candidates
				LockedCandidates();
														// if that didn't do anything, guess
				if (queue_depth<=0 && !invalid_board) Guess();
				}
			}
		if (invalid_board) PopStack();					// If contradiction, backup
		else if (cells_remaining<=0) {					// If solved, note it down
			++solution_count;
			if (solution_count==1 && result) {
				for (cell=0; cell<CELLS; ++cell) 
					*result++ = '1'+MaskToDigit(solution[cell]);
				*result=0;
				}
			if (solution_count>=max_solutions) break;	// Did we reach the limit?
			PopStack();									// No, try for others
			}
		}
	return(solution_count);
	}

/* END OF JSolve.c - A very fast Sudoku solver */
