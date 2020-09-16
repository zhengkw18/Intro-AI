import os
import sys
import json

table_path="../data/table.txt"
json_path="table.json"
dic={}

def loadTable():
    with open(table_path, 'r') as load_t:
        for line in load_t:
            pos=line.find(' ')
            pinyin_name=line[0:pos]
            list=[]
            for ch in line[pos:-1]:
                if '\u4e00' <= ch <= '\u9fff':
                    list.append(ch)
            dic[pinyin_name]=list
    with open(json_path, 'w+') as dump_t:
        json.dump(dic,dump_t)
if __name__=='__main__':
    loadTable()
    with open(json_path, 'r') as load_t:
        table = json.load(load_t)
        print(table)