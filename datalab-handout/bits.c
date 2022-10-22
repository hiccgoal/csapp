/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  // 使用真值表写表达式
  // x  y  x^y
  // 0  0   0
  // 0  1   1
  // 1  0   1
  // 1  1   0
  // 将值为1的式子写出来（即四行中的第二行和第三行）
  return (~x & y) | (x & ~y);
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  // 最小值,第一位为1,其余位为0即可
  return 1<<31;

}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  // !(~(x + 1) ^ x) ： 以16位为例,最大数为0x0111,对该数+1（0x1000）再取反（0x0001）,发现和初始位表示一样
  // 因此 ~(最大数+1) == 最大数,使用异或操作表达该关系即可
  // !!(x + 1) ： 然而0x1111也满足此条件,为将0x1111排除使用该式
  return !(~(x + 1) ^ x) & !!(x + 1);
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  // 只要x奇数位全部是1则返回1,那么x直接可以和奇数位全是1的数（allA=0xAAAAAAAA）进行异或运算
  // 结果是1,则说明x和allA不同,依题目要求,不同时return 1,对异或结果取反即可
  int allA = 0xAA;
  allA = allA | allA << 2;
  allA = allA | allA << 4;
  allA = allA | allA << 8;
  allA = allA | allA << 16;
  return !((allA & x) ^ allA);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  // 负数直接取反加1
  return ~x + 1
  ;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  // flag1：x前四位必须为3
  // flage2：x后四位必须为0~9的数字,0~9减10后是负数,第一位是1,可以利用这个特性
  int flag1 = !(x>>4 ^ 0x3);
  // x后四位减10后的结果(x & 0xF) + (~0xA + 1)
  // 将该结果和0x80000000相与,若结果为1,则说明后四位减10后为负数,即后四位0~9
  int flag2 = !!(((x & 0xF) + (~0xA + 1)) & 0x80 << 4);
  return flag1 & flag2;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  // x!=0时,x=1
  x = !!x;
  // 利用真值表得到 !x & z | x & y, 但这里的x只有一位,如何把x转换为32位,即
  // 0x0->0x00000000, 0x1->0xFFFFFFFF,我们使用取反操作即可完成该转换
  // 此时x=0x0时,flag=0x00000000,x=0x1时,flag=0xFFFFFFFF
  int flag = ~x + 1;
  // 注意运算符的优先级,若使用+而非|,下面两个括号必须有,否则会先计算y+~flag 
  return (flag & y) | (~flag & z);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  // 要求x<=y, 即y-x>=0时返回1,此时y-x的标志位flag=((y + (~x + 1)) >> 31)为0,取反即可
  // 则有真值表
  // x   y              返回值
  // 0   0    !((y + (~x + 1)) >> 31)
  // 0   1               0
  // 1   0               1
  // 1   1    !((y + (~x + 1)) >> 31)
  int a = x>>31 & 0x1;
  int b = y>>31 & 0x1;
  int flag = (y + (~x + 1)) >> 31;
  // 只需写出1,3,4行表达式即可,第3行为(a & ~b)
  // 1,4行为 (!(a ^ b) & (!flag))
  return (a & ~b) | (!(a ^ b) & (!flag));
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  // 要求实现!运算,观察到正数->0x0???,0->0x0000,负数0x1???
  // 只有0和自身相反数的符号位是一样的,因此
  // x=0时,(x | (~x+1)) = 0x0???
  // x!=0时,(x | (~x+1)) = 0x1???
  return ((x | (~x + 1)) >> 31) + 1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  int flag = (x >> 31);
  x = (flag & ~x) | (~flag & x);
  // 如果x高16位不为0,代表x至少需要0~15这16位,因此记录b16 = 16,并将x右移16位,计算16~31位需要几位
  // 如果x高16位为0,代表x不需要16~31这16位,因此记录b16 = 0,代表高16位需要0位,接下来计算8~15位是否需要
  int b16 = !!(x >> 16) << 4;
  x = x >> b16;
  int b8 = !!(x >> 8) << 3;
  x = x >> b8;
  int b4 = !!(x >> 4) << 2;
  x = x >> b4;
  int b2 = !!(x >> 2) << 1;
  x = x >> b2;
  int b1 = !!(x >> 1);
  x = x >> b1;
  int b0 = x;
  return b0 + b1 + b2 + b4 + b8 + b16 + 1;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  unsigned exp = uf & 0x7F800000;
  unsigned sign = uf & 0x80000000;
  unsigned frac = uf & 0x007FFFFF;
  // 非规格化数,直接左移一位,非规格化数左移后不用担心exp是全1的情况
  if (exp == 0) {
    return sign | uf << 1;
  }
  // 无穷或NAN
  if (exp == 0x7F800000) {
    return uf;
  }
  exp += 0x00800000;
  // 如果exp+1后变为全1,说明超过上限,此时设置frac全0,最终返回无穷
  if (exp == 0x7F800000)
  {
    frac = 0;
  }
  return sign | exp | frac;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  // v = (-1)^s * M * 2^E
  int sign = uf >> 31;
  // E = exp - bias(127)
  int E = ((uf >> 23) & 0xFF) - 127;
  // 上面两个要用int,因为可能是负数
  // M = 1 + f
  unsigned M = (uf & 0x007FFFFF) | 0x00800000;
  int value = 0;
  if (E < 0)
  {
    return 0;
  }
  else if (E > 31)
  {
    return 0x80000000u;
  }
  // 注意v = (-1)^s * M * 2^E的M = 1 + f = 1.xxxx
  // 而unsigned M实际上将v中M的值乘了2^23倍,也就是说现在的M是浮点数
  // 计算要使用的M的2^23倍,因此要做进一步处理
  else if (E < 23)
  {  
    value = M >> (23 - E);
  } else {
    value = M << (E - 23);
  }
  return sign? ~value + 1 : value;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  // v = (-1)^s * M * 2^E
  // 
  //                规格化的(s,!=0&!=255,f)                  非规格化的(s,00000000,f)
  // ----------------------------------------------------------------------------------
  //         |    E = exp - bias    |   M = 1 + f    |  E = 1 - bias  |    M = f  
  // ----------------------------------------------------------------------------------
  //  min    | exp = 00000001       |   f = 00...0   |                |   f = 00...01
  //         |   E = 1 - 127 = -126 |  M = 1 + 0 = 1 |    E = -126    |   M = 2^-23
  // ----------------------------------------------------------------------------------
  // M * 2^E |                   2^-126              |              2^-149
  // 
  // ----------------------------------------------------------------------------------
  //         |    E = exp - bias    |   M = 1 + f    |  E = 1 - bias  |    M = f
  // ----------------------------------------------------------------------------------
  //         |   exp = 11111110     |  f = 11111111  |               |   f = 111...1
  //  max    | E = 254 - 127 = 127  |  M = 2 - 2^23  |    E = -126   |   M = 1 - 2^-23
  // ----------------------------------------------------------------------------------
  // M * 2^E |              2^127 * (2 - 2^23)       |           2^-126 * (1 - 2^-23)
  // 
  // 可以得到以下区间
  //   x:        (-????,-150]       [-149,-127]           [-126,127]         [128,+????]
  //             对int来说太小   非规格化min,非规格化max  规格化min,规格化max   对int来说太大
  // return:           0                                                        +INF
  if (x <= -150)
  {
    return 0;
  } else if (x <= -127)
  {
    // 这时2^x为非规格化数,只需变化frac位即可
    // e.g.,
    // x=-149(1*2^-126*2^-23)时,浮点数位表示为s=0,exp=00000000,frac=00...01
    // x=-148(1*2^-126*2^-22)时,浮点数表示为s=0,exp=00000000,frac=00...10
    return 1 << (x + 149);
  } else if (x <= 127)
  {
    // 此时2^x为规格化数,并且frac不为0会导致类似2^?? * (2 - ? * 2^23)的存在，因此frac=0
    // e.g.,
    // 2^-126,s=0,exp=00000001,frac=00...0
    // 2^127,s=0,exp=111111110,frac=00...0
    return (x + 127) << 23;
  } else {
    // INF
    return 0xFF << 23;
  }
}
