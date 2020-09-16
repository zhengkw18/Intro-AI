import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
import json
import random
import torch.utils.data as data
from sklearn.metrics import f1_score
from scipy.stats import pearsonr
import math
import torch.backends.cudnn as cudnn

from models import CNN, RNN, BiLSTM, MLP

net_type = "CNN"
# gpus = [0, 1, 2, 3, 4, 5, 6, 7]
batch_size = 64
if net_type == "CNN" or net_type == "MLP":
    padding = 800
else:
    padding = 300
droprate = 0
if net_type == "MLP":
    regularization = 0.03
else:
    regularization = 0.05
epochs = 100
seed = 0

random.seed(seed)
np.random.seed(seed)
torch.manual_seed(seed)
torch.cuda.manual_seed_all(seed)

# Set up main device and scale batch size
device = 'cuda' if torch.cuda.is_available() else 'cpu'
# batch_size *= max(1, len(gpus))
print("Device:", device)

# Model
print('Building net...')
if(net_type == "CNN"):
    net = CNN(droprate)
if(net_type == "RNN"):
    net = RNN(droprate, padding)
if(net_type == "BiLSTM"):
    net = BiLSTM(droprate, padding)
if(net_type == "MLP"):
    net = MLP(droprate, padding)
if device == 'cuda':
    # net = torch.nn.DataParallel(net, gpus)
    # print("Using", len(gpus), "GPUs")
    net.to(device)
cudnn.benchmark = True

optimizer = optim.Adam(net.parameters(), weight_decay=regularization)

print("Loading training data...")
with open('./data/train-labels.json', 'r') as f:
    labelSet = json.load(f)

weight = [0.] * 8
for i in labelSet:
    weight[i] += 1

total = 0

for i in range(8):
    weight[i] = 1 / weight[i]
    total += weight[i]
total /= 8

for i in range(8):
    weight[i] /= total
if net_type == "CNN" or net_type == "MLP":
    loss_func = nn.CrossEntropyLoss(reduction='sum').to(device)
else:
    print("Weight:", weight)
    loss_func = nn.CrossEntropyLoss(reduction='sum', weight=torch.from_numpy(np.array(weight)).float()).to(device)

labelSet = torch.from_numpy(np.array(labelSet))
print("Traing label shape:", labelSet.shape)

with open('./data/train-300d.json', 'r') as f:
    inputSet = json.load(f)
    for i in range(len(inputSet)):
        if(len(inputSet[i]) < padding):
            inputSet[i].extend([[0.] * 300] * (padding - len(inputSet[i])))
        else:
            inputSet[i] = inputSet[i][:padding]
inputSet = torch.from_numpy(np.array(inputSet))
inputSet = torch.unsqueeze(inputSet, 1)
inputSet = inputSet.type(torch.FloatTensor)
print("Traing input shape:", inputSet.shape)

trainset = data.TensorDataset(inputSet, labelSet)
trainloader = data.DataLoader(trainset, batch_size=batch_size, shuffle=True)

print("Training data loaded")

print("Loading testing data...")
with open('./data/test-labels.json', 'r') as f:
    labelSet2 = json.load(f)
    for i in range(len(labelSet2)):
        labelSet2[i] = torch.from_numpy(np.array(labelSet2[i]))

with open('./data/test-labels2.json', 'r') as f:
    labelSet3 = json.load(f)
    for i in range(len(labelSet3)):
        labelSet3[i] = np.array(labelSet3[i])

with open('./data/test-300d.json', 'r') as f:
    inputSet2 = json.load(f)
    for i in range(len(inputSet2)):
        if(len(inputSet2[i]) < padding):
            inputSet2[i].extend([[0.] * 300] * (padding - len(inputSet2[i])))
        else:
            inputSet2[i] = inputSet2[i][:padding]
        inputSet2[i] = torch.from_numpy(np.array(inputSet2[i]))
        inputSet2[i] = inputSet2[i].type(torch.FloatTensor)

print("Testing data loaded")

trainlosses = []
testlosses = []

for epoch in range(epochs):
    print("Epoch", epoch)
    train_loss = 0
    net.train()
    for x, y in trainloader:
        optimizer.zero_grad()
        x = x.to(device)
        y = y.to(device)
        predict = net(x)
        loss = loss_func(predict, y)
        loss.backward()
        train_loss += float(loss.data)
        optimizer.step()
    train_loss /= len(trainloader.dataset)
    print("Train loss:", train_loss)
    trainlosses.append(train_loss)
    with torch.no_grad():
        test_loss = 0
        net.eval()
        numberOfTruePrediction = 0
        argmaxLabel, argmaxOutput = [], []
        sumOfCorrelationCoefficient = 0
        totalCounter = 0
        correlationCounter = 0
        for i in range(len(inputSet2)):
            hasNaN = False
            for x in labelSet3[i]:
                if math.isnan(x):
                    hasNaN = True
            if hasNaN:
                continue
            totalCounter += 1
            predict = net(inputSet2[i].view((1, 1) + inputSet2[i].shape).to(device)).detach()
            loss = loss_func(predict, labelSet2[i].view(-1).to(device))
            test_loss += float(loss.data)
            out = predict[0].cpu().numpy()
            numberOfTruePrediction += int(np.argmax(labelSet3[i]) == np.argmax(out))

            # F-score
            argmaxLabel.append(int(np.argmax(labelSet3[i])))
            argmaxOutput.append(int(np.argmax(out)))
            # Correlation Coefficient
            cc = pearsonr(out, labelSet3[i])[0]
            if(not math.isnan(cc)):
                correlationCounter += 1
                sumOfCorrelationCoefficient += abs(cc)
        print("Test loss:", test_loss / totalCounter)
        print('Accuracy:', numberOfTruePrediction / totalCounter)
        print('Macro f1:', f1_score(np.array(argmaxLabel), np.array(argmaxOutput), average='macro'))
        print('Correlation Coefficient:', sumOfCorrelationCoefficient / totalCounter)
        testlosses.append(test_loss / totalCounter)
    torch.cuda.empty_cache()

print(trainlosses)
print(testlosses)
