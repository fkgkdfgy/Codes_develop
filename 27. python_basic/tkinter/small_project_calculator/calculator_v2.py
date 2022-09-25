'''
Author: Liu Weilong
Date: 2021-02-15 09:56:13
LastEditors: Liu Weilong
LastEditTime: 2021-02-15 12:07:33
Description: 

v2 主要内容：
1. 限制 仅供数值输入     完成
2. listbox 显示添加     完成         
3. listbox 删除        完成
4. 重整代码结构          完成

'''

import tkinter as tk
import tkinter.messagebox

# 经过实验 .Tk 应该放在第一行代码
window = tk.Tk()

    
# 函数
def is_float(con):
    try:
        float(con)
        return True
    except:
        return False

def updateLB():
    global records
    listbox1.delete(0,'end')
    listbox2.delete(0,'end')
    total_price=0.0
    for i in records:
        listbox1.insert('end',i[0])
        listbox2.insert('end',i[1])
        total_price = total_price+ i[0]*i[1]
    vTotalPrice.set('总价：'+str(total_price))

def insert_record():
    global records
    vprice = vCurPrice.get()
    vquantity = vCurQuantity.get()
    print('add record successfully:')
    print('quantity: ',vquantity)
    print('price: ',vprice)
    if not(is_float(vprice)and is_float(vquantity)):
        tkinter.messagebox.showerror(title='错误', message='数量和价格的输入并不是数字')
        vCurPrice.set('')
        vCurQuantity.set('')
        return
    fq = float(vquantity)
    fp = float(vprice)
    records.append([fq,fp])
    vLableShow.set('小计：'+ str(fq*fp))
    vCurPrice.set('')
    vCurQuantity.set('')
    updateLB()

def delete_recent():
    global records
    records.pop()
    updateLB()

def delete_select():
    global records
    idx = listbox1.curselection()
    if len(idx)==0:
        idx = listbox2.curselection()
    if len(idx)==0:
        return
    for i in idx:
        records.pop(i)
    updateLB()

def clear_records():
    global records
    records.clear()
    updateLB()

# 变量
vCurQuantity = tk.StringVar()
vCurPrice = tk.StringVar()
vLableShow = tk.StringVar()
vLableShow.set('小计：')
vTotalPrice = tk.StringVar()
vTotalPrice.set('总价：')
records = []

# 组件

e_quantity = tk.Entry(window,textvariable = vCurQuantity)
e_price = tk.Entry(window,textvariable = vCurPrice)

e_quantity.pack()
e_price.pack()

b_add = tk.Button(window, text='添加记录', width=10,height=2, command=insert_record)
b_add.pack()

label = tk.Label(window,textvariable =vLableShow,bg='white', font=('Arial', 12), width=30, height=2)

label.pack()

label1 = tk.Label(window,text='数量')
label1.pack()
listbox1 = tk.Listbox(window)
listbox1.pack()
label2 = tk.Label(window,text='价格')
label2.pack()
listbox2 = tk.Listbox(window)
listbox2.pack()

label_total_price = tk.Label(window,textvariable = vTotalPrice,bg='white', font=('Arial', 12), width=30, height=2)
label_total_price.pack()

b_delete_recent = tk.Button(window, text='删除最近的一条', width=10,height=2, command=delete_recent)
b_delete_recent.pack()

b_delete_select = tk.Button(window, text='删除选择的内容', width=10,height=2, command=delete_select)
b_delete_select.pack()
b_clear = tk.Button(window, text='清空', width=10,height=2, command=clear_records)
b_clear.pack()
window.mainloop()

print(records)