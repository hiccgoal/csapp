这里面的文件一般是通过下面的命令得到的
首先将可执行文件转为`.s`文件：`objdump -d ctarget > ctarget.s`

在第二和第三个实验中，先写了`ctarget.l2.s`和`ctarget.l3.s`
```
gcc -c ctarget.l2.s
objdump -d ctarget.l2.o > ctarget.l2.d (也可以直接objdump -d ctarget.l2.o,反汇编的结果就在终端中)
```
创建一个`ctarget.l2.txt`,然后将攻击代码写在里面

在第五个实验中，使用了-Og优化选项
```
gcc -c -Og farm.c
objdump -d farm.o > farm.s
```
可以在这里面初步观察*“Gadget”* ，但注意在填入攻击代码的时候，*“Gadget”*的地址要在rtarget.s里面计算

### 测试方法：

```
./hex2raw < ctarget.l{}.txt | ./ctarget -q     {1,2,3}
./hex2raw < rtarget.l{}.txt | ./rtarget -q     {2,3}
```

### To do 1:

第五个问题，也就是`./hex2raw < rtarget.l3.txt | ./rtarget -q`时，第十一行需要执行 `mov %edx, %ecx`的操作，该操作对应的机器码是 `89 d1`.在`rtarget.s`中能找到以下4个*“Gadget”*满足 机器码中包含`89 d1`  的要求.
因此将rtarget.l3.txt 的11行的值换为下面四个*“Gadget”*中`89 d1`所处的位置均可.
但四个替换的测试结果如下：

1.

```
00000000004019f6 <getval_226>:
  4019f6: b8 89 d1 48 c0        mov    $0xc048d189,%eax
  4019fb: c3                    retq      
```
若换为：f7 19 40 00 00 00 00 00   一直fail

2.

```
0000000000401a33 <getval_159>:
  401a33: b8 89 d1 38 c9        mov    $0xc938d189,%eax
  401a38: c3                    retq          

```
若换为：34 1a 40 00 00 00 00 00  一直pass

3.

```
0000000000401a68 <getval_311>:
  401a68: b8 89 d1 08 db        mov    $0xdb08d189,%eax
  401a6d: c3                    retq           69 1a 40 00 00 00 00 00  
```
若换为：69 1a 40 00 00 00 00 00   pass都会出现fail

4.

```
0000000000401a6e <setval_167>:
  401a6e: c7 07 89 d1 91 c3     movl   $0xc391d189,(%rdi)
  401a74: c3                    retq         
```
若换为：70 1a 40 00 00 00 00 00   一直pass

### To do 2:
该机器使用的是小端法，在实验5(rtarget.l3.txt)第9行，为了让pop的值是0x48，输入的是48 00 00 00 00 00 00 00
类似的，利用字符串传参(cookie)，cookie `0x59b997fa` 作为字符串转换为ASCII为：`35 39 62 39 39 37 66 61`
，那么输入反过来应该是 `61 66 37 39 39 62 39 35`,但这样是错的
输入不应该是`fa 97 b9 59`吗?即`61 66 37 39 39 62 39 35`

#### 已解决：
```
#include <stdio.h>
typedef unsigned char* ptr;
void show_bytes(ptr bytes, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%.16x: %.2x\n",bytes+i, bytes[i]); // 地址+内容
    }
}
int main() {

    int ival;
    scanf("%d",&ival);
    show_bytes((ptr)&ival,sizeof(ival));

    printf("\n");

    char a[] = "12345678"; 
    show_bytes(a,8);
    
    return 0;
}
input:
12345678（0xbc614e)

output:
000000000061fe1c: 4e
000000000061fe1d: 61
000000000061fe1e: bc
000000000061fe1f: 00

000000000061fe13: 31
000000000061fe14: 32
000000000061fe15: 33
000000000061fe16: 34
000000000061fe17: 35
000000000061fe18: 36
000000000061fe19: 37
000000000061fe1a: 38
```
可以发现大小端对连续存储的数据满足数据的低字节放在低地址的形式
但对于字符串，顺序排布即可


### 参考资料：

[1] http://csapp.cs.cmu.edu/3e/attacklab.pdf 官方文档

[2] https://www.jianshu.com/p/db731ca57342 金际一线天

[3] https://www.cnblogs.com/xvic/p/15996206.html Deconx

[4] https://zhuanlan.zhihu.com/p/410418053 独小雪

[5] https://zhuanlan.zhihu.com/p/339802171 周小伦

[2]，[3]栈的可视化做的很好

[3]，[4]解释了第五个问题中0x48是怎么来的，为什么不是0x50

### 一些忽略的点：

%rsp和PC值的关系自己没有仔细思考过
csapp P270的ret指令的过程，先是写回R[%rsp]，然后是更新PC
想清楚这个才能知道第五个问题中0x48是怎么来的，为什么不是0x50