## 实验环境

#### 系统配置

Linux version 4.4.0-87-generic

Ubuntu 5.4.0-6ubuntu1~16.04.4

CUDA Version: 10.1

#### 软件配置

Python 3.7.3

Pytorch 1.5

CUDATookit 10.1

Numpy 1.18.1

Scikit-learn 0.23.1

Scipy 1.4.1

## 运行方式

- 进入code文件夹
- 从https://pan.baidu.com/s/1tUghuTno5yOvOx4LXA9-wg下载词向量压缩包，解压sgns.sogou.word至./data下
- 使用python transform.py生成词向量json
- 在train.py中更改模型类型（MLP、CNN、RNN、BiLSTM）
- 使用python train.py进行训练和测试