
/*
 * 系统调用表头文件
 *	简单实现sys_setup() sys_exit() sys_open() sys_read()等
 *		完整的内核支持的系统调用较多，本内核简单设置
 */

#define HZ 100

#define __NR_setup	0
#define __NR_exit	1
#define __NR_fork	2
#define __NR_read	3
#define __NR_write	4
#define __NR_open	5
#define __NR_close	6

#define __NR_pause	29


/* __NR_##name ---> __NR_fork */
/* 定义无形参的系统调用 */
#define _create_systemcall(type,name)\
type name(void)\
{\
	long __res;\
	__asm__ __volatile__("int $0x80"\
		: "=a"(__res)\
		: "0"(__NR_##name));\
	if(__res >= 0){\
		return (type)__res;\
	}\
	else{\
		return -1;\
	}\
}

/* 支持单个形参的系统调用 */


/* 支持两个形参的系统调用 */





/* 定义函数指针，代表各系统调用函数 */
typedef int (*syscall_ptr)();

extern int sys_setup();
extern int sys_exit();
extern int sys_fork(); /* 汇编实现，执行fork时被调用 */
extern int sys_read();
extern int sys_write();
extern int sys_open();
extern int sys_close();



