# Preprocessing the data and save the embeded word vectors as tensor

import json
import os
import random

# load Chinese word vector

wordVectors = {}

with open('./data/sgns.sogou.word', 'r', encoding='utf-8') as f:
    lines = f.readlines()
    for line in lines[1:]:
        splitLine = line.split(' ')
        wordVectors[splitLine[0]] = list(
            map(lambda x: float(x), splitLine[1:-1]))
vectors = list(wordVectors.values())

print('Chinese Word Vectors loaded.')


def transform(filename):
    labels = []
    inputs = []
    with open(os.path.join('./data', filename), 'r', encoding='utf-8') as f:
        lines = f.readlines()
        for line in lines:
            splitLine = line.split('\t')

            sentiments = splitLine[1].split(' ')

            maxSentiment, maxIndex = 0, -1
            for i, sentiment in enumerate(sentiments[1:]):
                s = int(sentiment.split(':')[1])
                if s > maxSentiment:
                    maxSentiment = s
                    maxIndex = i
                elif s == maxSentiment:
                    maxIndex = -1
            if maxIndex == -1:
                continue
            labels.append(maxIndex)

            words = splitLine[2].replace('\n', '').split(' ')
            currentWordVectors = []
            for word in words:
                if word in wordVectors:
                    currentWordVectors.append(wordVectors[word])
                else:
                    currentWordVectors.append(random.choice(vectors))
            inputs.append(currentWordVectors)
    with open(os.path.join('./data/', filename.split('.')[1] + '-labels.json'), 'w') as f:
        json.dump(labels, f)
    with open(os.path.join('./data/', filename.split('.')[1] + '-300d.json'), 'w') as f:
        json.dump(inputs, f)

# Transform `sinanews.demo`, `sinanews.test` and `sinanews.train` to word vector


transform('sinanews.demo')
transform('sinanews.test')
transform('sinanews.train')
