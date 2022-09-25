'''
Author: Liu Weilong
Date: 2021-02-15 09:16:37
LastEditors: Liu Weilong
LastEditTime: 2021-02-15 09:51:18
Description: 

v1 的主要作用：
1. 计算器想加
2. 显示结果
3. 自动清零

'''

import tkinter as tk

window = tk.Tk()

window.geometry('500x300')

v_q = tk.StringVar()
v_p = tk.StringVar()

e_quantity = tk.Entry(window,textvariable = v_q,show = None)
e_price = tk.Entry(window,textvariable = v_p,show = None)

e_quantity.pack()
e_price.pack()

records = []

v_label_1 = tk.StringVar()

def insert_record():
    global records
    vprice = v_p.get()
    vquantity = v_q.get()
    records.append([vquantity,vprice])
    print('add record successfully:')
    print('quantity: ',vquantity)
    print('price: ',vprice)
    fq = float(vquantity)
    fp = float(vprice)
    e_quantity.clipboard_clear()
    e_price.clipboard_clear()
    v_label_1.set('总价：'+ str(fq*fp))
    v_p.set('')
    v_q.set('')

b_add = tk.Button(window, text='添加记录', width=10,
               height=2, command=insert_record)

label = tk.Label(window,textvariable =v_label_1,bg='white', font=('Arial', 12), width=30, height=2)

b_add.pack()
label.pack()

window.mainloop()

print(records)
