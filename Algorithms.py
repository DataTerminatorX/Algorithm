﻿import math,random,copy
import itertools as it
import numpy as np

'''
目录
|--冒泡排序
|--快速排序
          |--递归实现
          |--非递归实现 ToDo
|--归并排序
|--打印逆序对儿(分治算法)
|--求数组中和最大的子数组
|--求字符串中的最大连续回文子串 ToDo
|--二叉树相关
          |--构建二叉树
          |--后续遍历
          |--树分层打印
          |--判断镜像树 ToDo
|--Viterbi算法(动态规划)
|--有向无环图节点排序
'''

# =================
# 冒泡排序
# 两层循环，内层循环逐个比较相邻俩元素，按比较条件决定是否交换次序

def bubble_sort(L):
    n=len(L)
    for i in xrange(n-1):
        for j in xrange(n-i-1):
            if L[j]>L[j+1]:
                L[j], L[j+1]=L[j+1],L[j] # 交换俩元素位置
# L=[4,1,9,7,5,3,6,8,2]
# bubble_sort(L)
# print L

# =================
# 快速排序(递归实现)

# 完美思路：在递归的基础上，空间复杂度降到O(1)，即i,j分别指向首尾，先挖出L[i]作为key，留个坑，然后j向左移，比key小就挖出来填到刚那个坑，这样留个坑，i向右移，比key大就填到刚刚的坑，ij如此交替直到相遇，相遇时必然指向了一个坑(i或者j之前挖的)，然后把key填到相遇的那个坑里就行了
# 这个思路其实是未知i,j会在那里相遇，但是相遇前，两边填坑的次数肯定是对称的，其效果就是把相遇点左边比key大的，扔到右边，右边比key小的，扔到左边
def quick_sort(L, low_index, high_index): 
    if low_index >= high_index: # 如果少于一个元素，返回
        return
    key=L[low_index] # 取第一个元素作为比较对象(即“轴”)
    i = low_index
    j = high_index
    toggle = 1 # toggle表示当前是移动i还是j
    while (i < j):
        if ( toggle): # 移动j
            if ( L[j] < key ): #当L[j]符合条件时，填进L[i]，j占坑L[j]，等着后面被i填
                L[i] = L[j]
                toggle = 0
                i +=1
            else: # 不符合条件时，继续向左遍历
                j -= 1
        else: # 移动i
            if ( L[i] > key ):
                L[j] = L[i]
                toggle = 1
                j -=1
            else:
                i+=1
    L[i] = key
    
    quick_sort(L, low_index, i-1)
    quick_sort(L, i+1,high_index)

# test code    
# L=[5,1,3,7,8,2,6,9,0,4,5,5,1,1]
# quick_sort( L, 0, len(L)-1)
# print L

# 非完美思路：在L中找一个数作为轴，比它(们)小的数放low_L，大的数放high_L，然后对俩再重复同样动作，直至每边只剩一个元素（多开了额外空间）
# 有点类似二叉树的搜索，用递归
def quick_sort_unperfect(L, low_index, high_index): # 这个方法开了额外的数组空间
    sub_L=L[low_index:high_index] #提取L中要排序的那块区域
    if len(sub_L)==1: # 如果只剩一个元素，返回
        return
    key=sub_L[0] # 取第一个元素作为比较对象(即“轴”)
    low_L=[]
    high_L=[]
    key_cnt=0 # 给sub_L中相同的key计数
    for i in xrange(len(sub_L)):
        if sub_L[i]==key:
            key_cnt+=1
        elif sub_L[i]<key:
            low_L.append(sub_L[i])
        else:
            high_L.append(sub_L[i])
    L[low_index:high_index]=low_L+[key]*key_cnt+high_L # 排序完替代L中原原始部分，因为函数参数是引用，因而可以改变实际的L
    if len(low_L) <>0:
        quick_sort(L, low_index, low_index+len( low_L ) ) # L中接下来要排序的区域
    if len(high_L) <>0:
        quick_sort( L, high_index-len(high_L), high_index )
# L=[4,1,5,8,9,1,3,2,2,6,7,8,0,5,2]
# quick_sort_unperfect( L, 0, len(L) )
# print L


# =================
# 快速排序(非递归实现)

# def quick_sort_nonrecur():


#=================
# 归并排序
# 1. 先二分成左右两部分 2. 左右两部分各自堆排序(递归调用函数)  3. 左右两边合并排序成一个有序列表
# tips1. 将L不停二分, 分到最后, 奇 = 1+2+2..., 偶 = 2+2+...
# tips2. 左右两个有序列表合并排序是关键, 具体见程序细节实现
def merge_sort(L, low_index, high_index):
    mid_index = low_index + (high_index-low_index)/2
    if (high_index - low_index ) > 1: # 即元素个数多于2个时，继续二分
        merge_sort(L, low_index, mid_index)
        merge_sort(L, mid_index+1, high_index)
    
    if high_index <> low_index: # 当L是奇数时会出现分到最后只有单个数的情形, 此时要返回, 只有不是单个数时, 才进行下面的排序
        # 合并两个有序列表变成一整个有序列表的细节:
        i = low_index 
        j = mid_index+1 # i,j 两个索引分别初始化为指向两列表最开始
        sub_L = [] # 用于存放排序结果
        while  ( len(sub_L) <> (high_index-low_index+1) ):
            if L[i] <= L[j]:
                sub_L.append( L[i] )
                if i <> mid_index:
                    i += 1
                else:
                    sub_L.extend( L[j:high_index+1] ) # 当一个列表索引到尾, 另一个如果当前元素比尾大, 则不用继续比较, 另一个的后面肯定都比尾大.
            else:
                sub_L.append( L[j] )
                if j <> high_index:
                    j += 1
                else:
                    sub_L.extend( L[i:mid_index+1] ) 
            
        L[ low_index: high_index+1 ] = sub_L
    return # return不是必须的, python会自动return None
    
# L=[4,1,9,7,5,3,6,8,2,2,0,9,1]
# merge_sort(L, 0, len(L)-1)
# print L

#=================
# 打印逆序对儿(分治算法)
# 输入: [3,1,4,2]  输出: [(3,1), (3,2), (4,2) ]
# 二分，左右打印逆序对儿(递归调用)，左右归并排序, 整体打印逆序对儿
# 在归并排序算法的基础上修改即可, 即在排序时加了打印逆序对儿的环节
def inverse_pair(L, low_index, high_index):
    result = []
    mid_index = low_index + (high_index-low_index)/2
    if (high_index - low_index ) > 1: # 即元素个数多于2个时，继续二分
        sub_result = inverse_pair(L, low_index, mid_index)
        result.extend(sub_result)
        sub_result = inverse_pair(L, mid_index+1, high_index)
        result.extend(sub_result)
    
    if high_index <> low_index: # 当L是奇数时会出现分到最后只有单个数的情形
    
        # 合并两个有序列表变成一整个有序列表并在此过程中打印逆序对儿
        i = low_index 
        j = mid_index+1 # i,j 两个索引分别初始化为指向两列表最开始
        sub_L = [] # 用于存放排序结果
        while  ( len(sub_L) <> (high_index-low_index+1) ):
            if L[i] <= L[j]:
                sub_L.append( L[i] )
                if i <> mid_index:
                    i += 1
                else:
                    sub_L.extend( L[j:high_index+1] ) # 当一个列表索引到尾, 另一个如果当前元素比尾大, 则不用继续比较, 另一个的后面肯定都比尾大.
            else:
                sub_L.append( L[j] )
                for k in xrange(i, mid_index+1):
                    result.append( ( L[k], L[j] ) ) #当左半数组的L[i]比右半数组L[j]大时，L[i]向后的每一个数，都可以与L[j]构成逆序对儿。
                if j <> high_index:
                    j += 1
                else:
                    sub_L.extend( L[i:mid_index+1] ) 
                    
        
        L[ low_index: high_index+1 ] = sub_L
    return result

# L = [3,2,1,5,4]
# print inverse_pair(L, 0, len(L)-1 )

#=================
# 求数组中和最大的子数组
# 算法思想见 review note 15.8.31, 总结在公众号文章『腾讯面经』里
# 输入[-3,5,2,1,7,-9,5,3,23,-111,3,7], 输出[5,2,1,7,-9,5,3,23]
def max_sub(L):
    i = 0
    itemp = 0
    j = 0
    k = 0
    max_sum = 0
    now_sum = 0
    max_neg = L[0] # 用来记录最大负数
    for idx in xrange(len(L)):
        now_sum += L[idx]
        if now_sum <= 0:
            now_sum = 0
            itemp = idx+1 # 记录和为正的子数组起始位置
        if now_sum > max_sum:
            max_sum = now_sum
            j = idx # 记录可能的最大和子数组结束位置
            i = itemp #记录可能的最大和子数组初始位置, 只有max_sum被改变, 和为正的子数组才可能是最大和子数组
        if L[idx] > max_neg:
            max_neg = L[idx]
            k = idx
    
    print i,j
    if max_sum == 0: # 说明都是负数
        return L[k], max_neg
    else:
        return L[i:j+1],max_sum

# L= [-8,-4,-9]
# L = [-3,5,2,1,7,-9,5,3,23,-500,3,7,-4]
# print  max_sub(L)

# ===============
# 求字符串中的最大连续回文子串
# 算法思想见 review note 15.9.2
# 输入: 




# ===============
# 二叉树相关
# 1.构建二叉树
class node(object):
    def __init__(self, value = None, left = None, right = None):
        self.value = value
        self.left = left
        self.right = right

# 2. 后续遍历
def Traverse_Bitree(T):
    if T.left <> None:
        Traverse_Bitree(T.left)
    
    if T.right <> None:
        Traverse_Bitree(T.right)
    
    print T.value

# 3. 树分层打印(类似图的广度优先搜索策略)
# 用一个队列, 先将根节点放进去, 然后: s1. 将目前处理的节点的左右孩子放到队列末尾 s2. 删除当前节点并处理下一个节点
def Print_Bitree_byLevel(T):
    L = [] # 类似队列, L.remove(L[0]) 可以去除队首元素, L.append(element)可以增添队尾元素
    L.append(T) #初始化
    while (len(L) <> 0):
        if L[0].left <> None:
            L.append(L[0].left)
        if L[0].right <> None:
            L.append(L[0].right)
        print L[0].value
        L.remove(L[0])

# test codes:   
# T = node(1, node(2, node(4, node(8), node(9)), node(5)), node(3,node(6,None,node(10)),node(7)))
# Traverse_Bitree(T)
# Print_Bitree_byLevel(T)

# 4. 判断镜像树
# 对于一个二叉树, 判断是否关于根节点对称
# 思路: 根节点的左右两块可以看做两个二叉树, 分别采用先序遍历和后序遍历的方法, 看结果是否相同

# ===============
# Viterbi算法
# 输入: 1. m行n列的二维list [[a11,...,a1m],...], a1m 表示第1列第m个节点值
#      2. m行m列的二维list, [[a11,...,am1], am1 表示第m个节点到第1个节点的路径值
# 输出: 每列取一个元素, 和最大的路径
# 思路: 略

def find_best_path(nodes, final_idx): # backward find best path
    best_path = [final_idx]
    current_idx = final_idx
    for v in nodes[::-1]:
        current_idx = v[current_idx]
        best_path.append(current_idx)
    return best_path[::-1]

def Viterbi(v_nodes, v_edges):
    m=len(v_nodes[0])
    best_nodes = [] # m行(n-1)列, 存储每步最优节点坐标
    for idx,v_node in enumerate(v_nodes):
        if idx==0:
            current_best_pathValue=copy.copy(v_node)
            previous_best_pathValue = copy.copy(current_best_pathValue)
        else:
            previous_best_node = []
            for node_idx in xrange(m):
                current_candidate_pathValue = map(sum, zip(previous_best_pathValue,v_edges[node_idx], [v_node[node_idx]]*m)) # add edge values
                print current_candidate_pathValue
                current_max_value = max(current_candidate_pathValue) # find max path value for now node
                current_best_pathValue[node_idx]= current_max_value # update best path value for now node
                previous_best_node.append(current_candidate_pathValue.index(current_max_value)) # update best previous node for now node
            best_nodes.append(previous_best_node)
            previous_best_pathValue = copy.copy(current_best_pathValue)
    final_best_nodeIdx = current_best_pathValue.index(max(current_best_pathValue))
    return find_best_path(best_nodes, final_best_nodeIdx)

def test_Viterbi(v_nodes, v_edges): # 暴力方法求解, 仅仅为验证上面结果是否正确
    n = len(v_nodes)
    m = len(v_nodes[0])
    best_value = -float('inf')
    best_path = 0
    for path in it.product(*[range(m)]*n): # 实现多重循环的功能，见 Evernote 20170306 2.
        path_value = 0
        for idx,node_idx in enumerate(path):
            if idx==0:
                path_value += v_nodes[idx][node_idx]
            else:
                path_value += v_nodes[idx][node_idx]+v_edges[node_idx][path[idx-1]]
        if best_value < path_value:
            best_value = path_value
            best_path = path

    return list(best_path)

# test codes:
# v_nodes = [[5,3,4,1],[1,1,50,9]]
# v_edges = [[1,1,1,1]]*4
# v_nodes = [[1,1,1]]*3
# v_edges = [[0,133,0],[100,13,1],[1,3,51]]
# v_nodes = np.random.randint(-10,10,size=(5,3)).tolist()
# v_edges = np.random.randint(-3,3,size=(3,3)).tolist()
# print test_Viterbi(v_nodes, v_edges)
# print Viterbi(v_nodes, v_edges)

# ===============
# 有向无环图节点排序
# 输入: 1. 根节点 [name1, name2, ...]
#      2. 其他节点 {(parent_name1,...): child_name1, (parent_name...): child_name2}
# 输出: 一个排好序的节点列表 [ name1, name2,...], 保证对一个孩子节点来说，它的父节点全部排在他前面
# 思路: 见印象笔记 20170418~20170420 3.

def ordered_directed_ayclic_graph(roots, G):

    ordered_nodes = roots[:]
    G1 = G.copy()
    now_parents = set(roots[:])
    while(G1 != {}):
        G2=G1.copy()
        parents=[]
        for k in G2: # traverse all remained parent nodes
            if G2[k] in ordered_nodes:
                raise ValueError('Graph contains circles!')
            if now_parents.issuperset(set(k)):
                G1.pop(k)
                ordered_nodes.append(G2[k])
                # print ordered_nodes
            else: parents+=list(k) # un-touched parents
        now_parents = set(ordered_nodes)&set(parents)

    return ordered_nodes

# test codes:
# G = {('1','2'):'4', ('3'):'5', ('4','3'):'6', ('4'):'7', ('7'):'8', ('1','8'):'9', ('6','9'):'10'}
# roots = ['1','2','3']
# print ordered_directed_ayclic_graph(roots, G)







