import os
import json

datasets_path="../datasets"
json_path="frequency.json"
dic={}
dic_freq={}
num_character=0

def addCh(ch):
    if ch in dic.keys():
        dic[ch]+=1
    else:
        dic[ch]=1

def loadDataset(dataset):
    global num_character
    print("Loading "+datasets_path+'/'+dataset)
    with open(datasets_path+'/'+dataset, 'r',encoding='UTF-8') as load_d:
        for line in load_d:
            lastlast='\0'
            last='\0'
            current='\0'
            for ch in line:
                lastlast=last
                last=current
                if '\u4e00' <= ch <= '\u9fff':
                    current=ch
                    num_character+=1
                    addCh(current)
                    if '\u4e00' <= last <= '\u9fff':
                        addCh(last+current)
                        if '\u4e00' <= lastlast <= '\u9fff':
                            addCh(lastlast+last+current)
                else:
                    current='\0'
def calculate():
    for key in dic.keys():
        if len(key)==1:
            dic_freq[key]=dic[key]/num_character
        elif len(key)==2:
            dic_freq[key]=dic[key]/dic[key[0]]
        elif len(key)==3:
            dic_freq[key]=dic[key]/dic[key[0:2]]

def loadFrequency():
    filelist=os.listdir(datasets_path)
    for dataset in filelist:
        loadDataset(dataset)
        print("Size of dic: "+str(len(dic)))
        print("Num of characters: "+str(num_character))
    calculate()
    with open(json_path, 'w') as dump_f:
        json.dump(dic_freq,dump_f)

if __name__=="__main__":
    loadFrequency()