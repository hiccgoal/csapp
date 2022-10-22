人做麻了
>make
>make test14 测试自己的
>make rtest14 官方solution
打印了进程走的路径a.txt,和make rtest4，tarce14.txt一起,三个文件对照看了两天,还是没看懂
1.  为什么`---cmdline:/bin/echo tsh> ./bogus`有两个
一个是123456(456在fork()后的子进程里面)
一个是12378(78父进程)
`---cmdline:/bin/echo tsh> ./bogus`不是应该执行一次吗,先是1,然后由于不是builtin_cmd,进入if后,23,然后是78456这几个数字出现吗,也就是说12378546里面,123是产生子进程之前的,456是子进程的,78是父进程的
```
---cmdline:/bin/echo tsh> ./bogus

evalhere1
 here 1 ;
 here 2 ;
 here 3 ;
 here 7 ;
 here 8 ;
evalhere2
---stdouthere


---cmdline:./bogus

evalhere1
 here 1 ;
 here 2 ;
 here 3 ;
 here 4 ;
 here 5 ;
 here 6 ;
./bogus: Command not found
---stdouthere
 ```
2. 另一个迷惑的地方:
```
---cmdline:./myspin 4 &

evalhere1
 here 1 ;
 here 2 ;
 here 3 ;
 here 7 ;
 [1] (2865) ./myspin 4 &
 here 9 ;
evalhere2
tsh> fg
---stdouthere
```
`cmdline:./bogus`走123456；`./myspin 4 &`为什么不走123456,后者是创建了子进程,将其放在后台,即不等待该命令即可执行其他命令.显然后者也创建了子进程,从a.txt来看,后者貌似并没有走到子进程里的printf语句

3. 一个注意的点是`myspin.c`程序,可以在终端试试sleep,会发现时间到了后会回到终端,这应该对应信号里面的20,SIGTSTP,即来自终端的停止信号.这和 sigtstp_handler 检测的SIGTSTP应该不同,前者是子进程停止发出,被shell检测的信号,在sigchld_handler里面处理,后者是直接发送给shell的,(可以看tsh.c里的comment),类似于在终端输入ctrl+z,会直接退出终端

4. 直接写程序感觉还行,直接把有些东西看作黑盒子就行了,但打印后,陷入这些细节里很多东西真滴不太懂,心累....

https://www.cnblogs.com/zhiyiYo/p/16297819.html
https://zhuanlan.zhihu.com/p/119034923
https://zhuanlan.zhihu.com/p/89224358
https://www.bilibili.com/video/BV1EF411h791/?p=12&spm_id_from=pageDriver&vd_source=d35440920cc2cf998019ffb3c92d2f79
https://www.cnblogs.com/zhangyi1357/p/16005508.html
https://github.com/zhoudiqiu/Shell-lab/blob/master/lab5/tsh.c
https://www.jianshu.com/p/7f5e78e83a0e