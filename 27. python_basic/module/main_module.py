'''
Author: Liu Weilong
Date: 2021-02-14 18:13:26
LastEditors: Liu Weilong
LastEditTime: 2021-02-14 18:17:42
Description: 
'''
import sys
from self_package import self_function


if __name__ == '__main__':
    for i in sys.path:
        print(i)

    self_function.print_info(12323,2323)
else:
    print("not main module")
    