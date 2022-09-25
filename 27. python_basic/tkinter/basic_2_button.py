'''
Author: Liu Weilong
Date: 2021-02-15 08:39:51
LastEditors: Liu Weilong
LastEditTime: 2021-02-15 09:05:35
Description: 
'''

import tkinter as tk

window  = tk.Tk()
window.geometry('500x300')

var = tk.StringVar()

label = tk.Label(window,textvariable = var,bg='green', fg='white', font=('Arial', 12), width=30, height=2)

label.pack()

on_hit = False

def hit_me():
    global on_hit
    if on_hit == False:
        on_hit = True
        var.set('you hit me')
    else:
        on_hit = False
        var.set('')


button = tk.Button(window, text='hit me', font=('Arial', 12), width=10, height=1, command=hit_me)
button.pack()

window.mainloop()