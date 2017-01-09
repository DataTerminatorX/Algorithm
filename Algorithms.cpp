#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

/*
目录
|--快排
|--翻转字符串中的单词位置
|--有奇有偶数的数组，输出使奇数在前，偶数在后
|--输入只含有三种数字1,2,3的数组a，输出它们的排序结果
|--0~n-1这n个整数乱序放在a[n]的数组中，由小到大排序
|--给定一个含有n个元素的整型数组a，其中有个元素出现次数超过n/2，求出这个元素
|--两字符串最长公共子序列(动态规划) todo
|--最长递增子数组(动态规划)
|--有n个台阶，一次爬1或2级，有几种爬法
*/

//===============================
// 快排的c实现，思路见algorithm.py
void quick_sort(int a[],  int low, int high)
{
    if (low>=high)
        return;
    int key = a[low], i = low, j = high;
    bool toggle = 1;
    while (i<j)
    {
        if (toggle)
        {
            if ( a[j] < key)
                { a[i] = a[j]; i++;toggle = 0;}
            else
                j--;
        }
        else
        {
            if ( a[i] > key)
            { a[j] = a[i]; j--; toggle = 1;}
            else
                i++;
        }
    }
    a[i] = key;
    quick_sort(a, low, i-1);
    quick_sort(a, i+1, high);
}
//===============================
// 翻转字符串中的单词位置, 如输入"ha mo"，输出"mo ha"
// 思路：先变成"ah om"，即单词位置不变单词本身翻转，再整个翻转得到结果
void inverse_word(int j, int i, string &s) //将s[i]~s[j]翻转
{
    char a;
    while(i > j)
    {a = s[i]; s[i] = s[j]; s[j] = a;i--;j++;}
}
void inverse_words(string &s)
{
    int i = 0, j = 0;
    
    // 翻转每个单词
    while(s[i] != '\0')
    {
        i++;
        if ( s[i] == ' ' || s[i] == '\0')
        {
            inverse_word(j, i-1, s); // j记录单词词首
            j = i+1;
        }
    }
    
    inverse_word(0, i-1, s); //翻转整个句子
}

// 相似题：数组循环左移k位，如左移3位，输入 123456，输出456123
void inverse_array(int *i, int *j)
{
    int temp, *p = i , *q = j;
    while(p < q)
    {temp = *p; *p = *q; *q = temp; p++; q--;}
}
void left_shifting_array(int a[], int n, int k)
{
    int *p = a, *q = &a[k-1], *r = &a[n-1];
    inverse_array(p,q);
    inverse_array(q+1,r);
    inverse_array(p,r);
}

//=================================
// 输入一个有奇有偶数的数组，输出使奇数在前，偶数在后，奇数(偶数)间的相对位置可变
// 思路：俩指针分别，一个从前向后，一个从后向前。当前面指针指的是偶数时，后面指针向前移动直到指向奇数，两者指向的值交，然后前面指针继续向后，循环前面步骤。直到两者指向同一位置
void split_odd_even(int a[], int n) // c语言的数组都是传指针，所以不用&来引用，也可以 int *a
{
    int *front = &a[0], *rear = &a[n-1], temp;
    while (front < rear)
    {
        if ( (*front %2 == 0) && (*rear %2 == 1))
            {temp = *front; *front = *rear; *rear = temp;}
        while (*front %2 != 0) //定位到奇数
            front++; 
        while (*rear %2 != 1) // 定位到偶数
            rear--;
    }
}


//================================
// 同上，但奇数(偶数)间的相对位置不可变
// 思路：


//================================
//输入只含有三种数字1,2,3的数组a，输出它们的排序结果，如输入3, 1, 2, 3, 1, 1, 2，输出 1, 1, 1, 2, 2, 3, 3
//思路一、时间复杂度o(2n)
//1. 用俩指针rear和front初始化为a[0] 
//2. 对所有的3进行merge，具体地，rear指向3这个区块得最左边，front指向最右边，当往右遍历遇到非3的数字时，跟rear交换，继续前进，直到front遇到最末尾的end指针，此时记录下rear指向的值赋给end 
//3.继续对2merge，重复1.2.步骤，此时end指针并不是数组末尾，而是merge好的3的左边
// 这个思路比较通用，可以扩展到含有1~k个数字的数组
void swap(int *a, int *b)
{
    int temp;
    temp = *a; *a = *b; *b = temp;
}
void sort_3number_array(int a[], int n) 
{ 
    int i,*front, *rear, *end = & a[n-1];
    for (i = 3; i > 1; i--)
    {
        rear = a; front= a; // 初始化
        while(front != end)
        {    
            front++; // 始终指向当前i区块的下一个
            if (*front != i)
            {   
                swap(front, rear);
                rear++; // 始终指向当前i区块的最左边
            }
        }
        end = rear-1; 
    }    
}
//思路二、时间复杂度o(n)
// 1.  俩指针left和right定界，left是指向从左往右第一个非"1"的数，right指向从右往左第一个非"3"的数
// 2.  从left开始用指针now遍历，当now指向的数是2时，不操作，继续遍历；当遇到3时，跟right指针指向的数交换，right左移，若交换过来是1，再跟left交换，left右移动，若遇到3，继续
// 这个思路不够通用，当不止三种数的时候没法套
/* （来自网上）总结一下上面的算法：
p1(即我这里的left)从左侧开始，指向第一个非1的数字；p3(我这里的right)从右侧开始，指向第一个非3的数字。
p2(now)从p1开始遍历，如果是2，p2继续遍历，直到p2遇到1或者3
如果遇到1，则和p1进行交换，然后p1向右，指向第一个非1的数字
如果遇到3，则和p3进行交换，然后p3向左，指向第一个非3的数字
重复上面的步骤，直到p2在p3的右侧结束。 */

void sort_3number_array_2(int a[], int n)
{
    int *left = a, *right = &a[n-1], *now;
    while(*left ==1) left++;
    while(*right == 3) right--;
    now = left;
    while(now <= right)
    {
        while(*now != 2)
        {
            if (*now == 1)
            {
                swap(now, left); 
                while(*left ==1) left++; // left重新定位到左边往右第一个非"1"值
            }
            else
            {
                swap(now, right); 
                while(*right == 3) right--; //同上
            }
            now = left; // now重新定位到left
        }
        now++; 
    }
    
}

//===============================
// 题目来源及解析：http://blog.csdn.net/morewindows/article/details/8204460
// 0~n-1这n个整数乱序放在a[n]的数组中，由小到大排序，时间复杂度o(n)
// 思路：从a[0]开始，如果a[0]里面放的是0，继续往后处理a[1]；如果不是，假设a[0]里面放的是i，就拿 i 和 a[i] 里面的数交换，假设a[i]里放的是j，交换后，a[1]里放的是 a[ a[i]]，也就是j，而a[i]里面放的是i， 如果 j不是0，继续这个交换步骤，即拿 j和a[j]里放的数交换，知道交换到0过来为止。然后继续向后处理a[1]，直到a[1]=1，然后继续向后处理，一直处理到a[n-1]。最少交换次数是1次，对应本来就
// 总结：来源于“基数排序”，如果把a[0]~a[n-1]看做n个桶，哈希函数就是a[i] = i，所做的就是把乱序的0~n-1 分别哈希到a[i]里去。只不过这里没有重新开辟哈希表空间，而是巧妙地先用a[0]做中转站，把a[0]里的数i哈希到a[i]，再取出a[i]里的数j暂时放到a[0]，然后继续对j做哈希。更严谨地说，是利用a[i] != i时的a[i]作为中转站
void n_array_sort(int a[], int n)
{
    int i = 0;
    for (i; i<n; i++)
    {
        while( (a[i] != i))
        {
            swap(&a[i], &a[a[i]]);
        }
    }
} 

// 将上面题目改成，由大到小排序
// 思路：类似上面，只需将哈希函数改成 a[i] = n-1-i
void n_array_sort_inverse(int a[], int n)
{
    int i = 0;
    for (i; i<n; i++)
    {
        while( (a[i] != n-1-i) )
        {
            swap(&a[i], &a[n-1-a[i]]);
        }
    }
} 

// n个数乱序放在a[n]的数组中，数的范围是0~n-1，但可能有重复的。要求找出所有重复的数字，时间复杂度o(n)
// 思路：类似上面，用哈希的角度思考，如果有重复，就会发生哈希冲突，比如 把a[0]里的 i 哈希到 a[i] 时，发现 a[i] 里面已经有 i 了，说明i 重复了，这时候把a[0] 里面放-1就好了。
void n_array_multi(int a[], int n)
{
    int i = 0;
    for (i; i<n; i++)
    {
        while( (a[i] !=i) && (a[i] != -1) )
        {
            if (a[a[i]] == a[i] ) // 哈希冲突  
            {
                cout << a[i]<<endl;
                a[i] = -1;
            }
            else swap(&a[i], &a[a[i]]);
        }
    }
} 

//==============================
//（“寻找发帖水王”）给定一个含有n个元素的整型数组a，其中有个元素出现次数超过n/2，求出这个元素
// 思路1：数学之美上的，类似两两比较数对儿，思路见笔记，时间复杂度o(n/2)吧我猜
int find_nonregular_id(int a[], int n)
{
    int nowid=a[0], count=1,i = 0;
    while (++i != n)
    {
        if (count == 0)
            nowid = a[i];
        if (nowid == a[i]) 
            count++;
        else
            count--;
    }
    return nowid;
}

//===============================
// 两字符串最长公共子序列
// 思路：动态规划的套路，用dp[i][j]代表 字符串1的前i个与字符串2的前j个的公共子序列的长度，核心公式:dp[i] == dp[j]时, dp[i][j] = dp[i-1][j-1]+1; !=时, dp[i][j] = max(dp[i-1][j], dp[i][j-1])
// 形象化的理解见笔记
string max_subsequence(string &s1, string &s2)
{
    int n1,n2;string result = "";
    n1 = s1.length();
    n2 = s2.length();
    int dp[n1][n2],i,j;
    s1[0] == s2[0]? dp[0][0] = 0:dp[0][0]=1;
    for (i=0;i<n1;i++) //初始化dp矩阵第一列，即拿s1和s2[0]匹配
    {
        if (s1[i] == s2[0])
            dp[i][0] = 1;
        else
            dp[i][0] = dp[i-1][0];
    }
    for(i=0;i<n2;i++) // 初始化dp矩阵第一行，即拿s1[0]和s2匹配
    {
            if (s1[0] == s2[i])
            dp[0][i] = 1;
        else
            dp[0][i] = dp[0][i-1];
    }
    
    for (i=1;i<n1;i++)
        for(j=1;j<n2;j++)
                s1[i] == s2[j]? dp[i][j] = dp[i-1][j-1]+1 : dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
        
}


//===============================
// 最长递增子数组
// 求一个整数数组中长度最长的递增数组，如输入 [3,1,4,2,5]，分别实现 1.只输出一组，如[3,4,5] 2. 输出所有的，如[3,4,5] 和 [1,2,5] 
// 思路: 动态规划的思想, 对于第i个数A[i], 用L[i]代表以A[i]结尾的最大递增子数组的长度, 则问题等价转换为：
//      求 L[1]~L[n]中的最大值，然后根据这个最大值(一个或多个) L[j]对应的记录回溯最长递增数组（跟Viterbi算法异曲同工），具体地：
//      L[0]=1，然后令j从0到i-1, 则有递推式: 若A[i]>A[j]，则L[i]=max(L[j]+1)、对于所有j，并记录下对应的索引j(有多个则都记录)，若全无，则 L[i] =1(记录索引i)
//      找出最大的L[j]，并回溯最长递增数组，如果有多个最大值，则对每一个最大值 L[j]，找出其记录的所有索引，并根据索引逐个回溯，有点类似一个多叉树，可以用递归的思想打印所有子数组
// 改进点(ToDo): 每轮比较当中有冗余比较, 可以先给 A 进行排序, 这样每次比较 A[i] 和 A[0]~A[i-1] 时，只需要比较比 A[i] 小的即可

// 只输出一组最大长度递增子数组 (有bug，继续调)
void max_ascend_substr(int a[], int n)
{

    int states[n][2];
    int i,j;
    int state[2]={1,0}; // state[0] 用来记录 L[i], state[1] 用来记录索引i
    for (i=0; i<n; i++)
    {
        if(i==0)
        {
            state[1]=0;
        }
        else
        {
            state[1]=i;
            for(j=0; j<i; j++)
            {
                if(a[i]>a[j])
                {
                    if(states[j][0]+1 >= state[0])
                    {
                        state[0] = states[j][0]+1;
                        state[1] = j;
                    }
                }
            }
        }
        states[i][0] = state[0];
        states[i][1] = state[1];
    }
    // 输出
    int b[2]={0,0};
    for (i=0; i<n; i++)
    {
        if(states[i][0]>b[0])
        {
            b[0] = states[i][0];           
            b[1] = states[i][1];           
        }
    }
    i = b[1];
    cout<<a[i]<<endl;
    while(states[i][1] != i)
    {
        i = states[i][1];      
        cout<<a[i]<<endl; 
    }
}

// ToDo 输出多组拥有同样最大长度的递增数组(要用 record 额外记录第 i 个数的 i-1 次比较的结果, 以找出多个最大值, 输出时需要用递归的思想打印)
/*
void max_ascend_multi_substr(int a[], int n)
{

    vector<vector<int>> states(n);
    int i,j;
    vector<int> state; // state[0] 用来记录 L[i], state[1:end] 用来记录索引i
    vector<int> record; // record[0~i-1] 用来记录每个 L[j]+1 
    for (i=0; i<n; i++)
    {
        state.clear();
        state.push_back(1);
        if(i==0)
        {
            state.push_back(0);
            states.push_back(state);
        }
        else
        {
            state.push_back(1);
            state.push_back(i)
            for(j=0; j<i; j++)
            {
                if(a[i]>a[j])
                {
                    if(states[j][0]+1 <= state[0])
                    {
                        state[0](states[j][0]+1)
                        state.push_back()
                    }
                }
            }
            states.push_back(state)
        }
    }

    //Todo: 打印多个最大长度数组 
}
*/


//===============================
// 有n个台阶，一次爬1或2级，有几种爬法
// 思路1.(我的)：类似一个二叉树，每个节点指向1或2，直到该节点累积爬的步数达到n，则计数器+1
void upstairs(int n_remain, int &count)
{
    if (n_remain == 0)
    {
        count++; //剩余步数为0时，计数器+1
    }
    else
    {
        if(n_remain > 0 )
        {
            upstairs( n_remain-1,count); // 爬1步或2步，类似左右孩子
            upstairs(n_remain-2,count);
        }
    }
    return; // 剩余步数<0时也结束
}
// 思路2. 用递归的思想 f(n) = f(n-1) + f(n-2), 即走到第n个台阶所用方法, 是走到第n-1 个台阶和第 n-2个台阶的和 (离第n个分别是1步和2步)
// 思路3. 斐波那契数列？

int main()
{
    /*
     int n =7,i=-1;
    int a[n] = {1,3,7,2,5,6,4};
    quick_sort(a, 0, n-1);
    while (++i<n)
    cout<<a[i]<<endl; 
    */

/*     string s ="miao is not mao";
    inverse_words(s);
    cout<<s<<endl; */

/*     int n =7,i=-1;
    int a[n] = {1,2,3,4,5,6,7};
    left_shifting_array(a, n, 3);
    while (++i<n)
    cout<<a[i]<<endl; */
    
/*     int n =7,i=-1;
    int a[n] = {1,3,2,5,4,7,9};
    split_odd_even(a, n);
    while (++i<n)
    cout<<a[i]<<endl; */
    
/*     int n =11,i=-1;
    int a[n] = {3,2,1,1,2,2,1,3,3,2,1};
    sort_3number_array_2( a ,n);
    while (++i<n)
    cout<<a[i]<<endl; */
    
/*     int n =5,i=-1;
    int a[n] = {1,2,3,4,0};
    n_array_sort_inverse(a, n);
    while (++i<n)
    cout<<a[i]<<endl; */
    
/*     int n =8,i=-1;
    int a[n] = {3,4,4,4,0,0,2,3};
    n_array_multi(a, n); */
    
/*     int n = 8;
    int a[n] = {2,2,3,2,1,1,2,1};
    cout<< find_nonregular_id(a, n)<<endl; */
    
/*  int n = 5,count = 0;
    upstairs(n,count);
    cout<<count<<endl; */

int n=5;
int a[5] = {1,2,3,4,5};
max_ascend_substr(a, n);
}


