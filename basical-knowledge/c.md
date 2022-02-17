==等于判断符
！=不等于判断符
（真为1，假为0）
&位运算符，表示按位与   1010&1011 =1010
|位运算符，表示按位或     1010 | 1011 =1011
~位运算符，按位取反
\在最后表示换行
&&逻辑与            ？条件运算符	||逻辑或
%取余运算符
a& = ~b        :   a = a&(~b)，将a、b转为2进制，b取反再与a与
const 只讀，只在聲明中使用    只读的全局变量,其值不可修改.
static 靜態存儲方式   规定此全局变量只在当前模块(文件)中可见.
extern 申明變量
b##a      b和a名字連在一起
？ ：    ？表示if语句，：表示else语句
go to Start語句：直接去向名字
void *memcpy(void *str1, const void *str2, size_t n)
从存储区 str2 复制 n 个字节到存储区 str1
sprintf (str_temp, "%d", motor[0].mode)   发送格式化输出到 str 所指向的字符串
strcpy((char *)(&usart.TxBuffer_USART2[i]), str_temp);//把后者所指向的字符串复制到前者
strlen(str_temp);//计算字符串 str_temp的长度

 (align(m)即这个地址值一定是m的整数倍。并且其占用的空间，即大小,也是m的整数倍
 __attribute__ 可以设置函数属性（Function Attribute ）、变量属性（Variable Attribute ）和类型属性（Type Attribute ） 
 可以对结构体（struct ）或共用体（union ）进行属性设置。大致有六个参数值可以被设定，即：aligned, packed, transparent_union, unused, deprecated 和 may_alias 

static inline void 
希望全局使用又希望增加效率的函数实现在头文件

结构体指针 -> 成员名
volatile  ①用于中断服务中修改供其他程序检测的变量②多任务环境下各任务共享的标志
int a,b,*p1,*p2,*p;
p1=&a;指针p1指向a的地址
p2=&b;指针p2指向b的地址

引用数组元素：
①下标法STUDENT[i]
②指针法*（a+i）或*(p+i)
③通过数组名计算数组元素地址 

二维数组指针变量：
类型说明符（*指针变量名）[长度]

字符串与指针
char string[]="helloworld"
字符指针
char *string="helloworld"

字符串的存取方法：下标法、指针法
下标法
char string[]="helloworld"
for(i=0;*(string+i)!='\0';i++)
{*(b+i)!=*(a+i);}
*(b+i)='\0';

指针法
int *p1,*p2,a,b;
for(;*p1!='\0';p1++,p2++)
{*p2=*p1;}
*p2='\0';

指针变量和数组变量调用原理不同，
指针变量放在常量存储区，只能读，不能写。

五大内存分区：堆、栈、自由存储区、全局存储区和常量存储区

字符指针变量和字符数组
字符数组由若干个元素组成，每个元素放一个字符，字符指针变
量存放的是字符串第一个字符的地址

指向函数的指针：函数的入口地址
带回指针值的函数int *a(int x,int y);

c语言嵌套注释会错乱
#if预编译，判断编译不编译
#endif

 宏来代表简短的表达式比较合适 
宏定义写函数： (宏没有返回值和返回类型)
#define M(A,B)\
do{\
语句1；\
语句2；}while(0)    \

内联函数inline( 实质是**以空间换时间** ):减少了进出函数的时间
关键字inline 必须与函数定义体放在一起才能使函数成为内联

函数定义前加static:  指对函数的作用域仅局限于本文件

uint：无符号数，0-65535
int：有符号数，-32768-32767
_IO:静态 即volatile
_I：只读访问
_O:只写访问
enum：枚举类型

#### 状态机

状态(现态和次态)，事件(条件)，动作
确定性状态机：有限的状态；有限的输入事件；有确定的状态迁移规则

​	typedef enum lagori_state{
catch_success,
catch_failed,
release_success,
release_failed};LAGORI_STATE ;
​	typedef struct  lagori_num{
LAGORI_STATE lagori_state;
void (*ctrloop)(void);//执行函数
void (*ctrlint)(void);//初始化函数
}LAGORI_NUM;
​	LAGORI_NUM  lagori_catch_and_release[] = {
{},
{},
{lagori3,catch,},
};

