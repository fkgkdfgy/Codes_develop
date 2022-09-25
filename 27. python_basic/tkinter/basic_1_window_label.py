'''
Author: Liu Weilong
Date: 2021-02-14 19:52:12
LastEditors: Liu Weilong
LastEditTime: 2021-02-15 08:39:06
Description: 
'''
import tkinter as t 

window = t.Tk()

window.title("My Window")
window.geometry('500x300')
# 这里声明是在window上面
l = t.Label(window, text='你好！this is Tkinter', bg='green', font=('Arial', 12), width=30, height=2)
# 说明： bg为背景，font为字体，width为长，height为高，这里的长和高是字符的长和高，比如height=2,就是标签有2个字符这么高
l.pack()  
window.mainloop()
