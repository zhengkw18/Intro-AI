import os
import sys
import json

input_path="../data/input.txt"
output_path="../data/output.txt"
table_path="table.json"
json_path="frequency.json"
correct_output_path="../data/output_.txt"
dic={}
table={}
lamb=0.01
lamb1=0.2
lamb2=0.79
output=''
correct_output=''

def getFrequency(word):
    if word in dic.keys():
        return dic[word]
    else:
        return 0.0

def readFrequency():
    global dic
    with open(json_path, 'r') as load_f:
        dic = json.load(load_f)

def readTable():
    global table
    with open(table_path, 'r') as load_t:
        table = json.load(load_t)
def convert():
    global output
    with open(input_path, 'r') as load_i:
        for line in load_i:
            dp = []
            line=line.lower()
            output_line=''
            pinyin_list=line.strip(' ').replace('\n','').replace('\t','').split(" ")
            for pinyin in pinyin_list:
                if pinyin=='':
                    continue
                dic_now={}
                if len(dp)==0:
                    for ch in table[pinyin]:
                        dic_now[ch]=['',getFrequency(ch)]
                    dp.append(dic_now)
                elif len(dp)==1:
                    dic_last = dp[-1]
                    for ch in table[pinyin]:
                        pmax=0.0
                        argmax=list(dic_last.keys())[0]
                        for ch_last in dic_last.keys():
                            p_last=dic_last[ch_last][1]
                            p=p_last*(lamb*getFrequency(ch)+(1-lamb)*getFrequency(ch_last+ch))
                            if p>pmax:
                                pmax=p
                                argmax=ch_last
                        dic_now[ch]=[argmax,pmax]
                    dp.append(dic_now)
                else:
                    dic_last=dp[-1]
                    for ch in table[pinyin]:
                        pmax=0.0
                        argmax=list(dic_last.keys())[0]
                        for ch_last in dic_last.keys():
                            p_last=dic_last[ch_last][1]
                            p=p_last*((1-lamb1-lamb2)*getFrequency(ch)+lamb1*getFrequency(ch_last+ch)+lamb2*getFrequency(dic_last[ch_last][0]+ch_last+ch))
                            if p>pmax:
                                pmax=p
                                argmax=ch_last
                        dic_now[ch]=[argmax,pmax]
                    dp.append(dic_now)
            dic_final=dp[-1]
            pmax=0.0
            argmax=list(dic_final.keys())[0]
            for ch in dic_final.keys():
                p=dic_final[ch][1]
                if p>pmax:
                    pmax=p
                    argmax=ch
            ch=argmax
            for dic_now in dp[::-1]:
                output_line=ch+output_line
                ch=dic_now[ch][0]
            output=output+output_line+'\n'

def testLambda():
    global lamb1,lamb2,output,correct_output
    lambs=[0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9]
    with open(correct_output_path, 'r') as load_c:
        correct_output=load_c.read().replace(' ','').replace('\n','').replace('\r','')
    for lam in lambs:
        output=''
        lamb1=lam
        lamb2=0.99-lam
        print("Testing lambda1=" + str(lamb1)+" lambda2="+str(lamb2))
        convert()
        output=output.replace(' ','').replace('\n','').replace('\r','')
        assert len(output)==len(correct_output)
        length=len(output)
        cnt=0
        for i in range(length):
            if output[i]==correct_output[i]:
                cnt+=1
        print("Accuracy: "+str(cnt/length))

if __name__=='__main__':
    if len(sys.argv)==3:
        input_path=sys.argv[1]
        output_path=sys.argv[2]
    readFrequency()
    readTable()
    test=False
    if test:
        testLambda()
    else:
        convert()
        with open(output_path, 'w+') as save_o:
            save_o.write(output)