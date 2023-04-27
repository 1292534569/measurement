# 2023-4-25  测试三种滤波
- [2023-4-25  测试三种滤波](#2023-4-25--测试三种滤波)
  - [原图](#原图)
  - [无滤波](#无滤波)
  - [均值滤波](#均值滤波)
  - [中值滤波](#中值滤波)
  - [高斯滤波](#高斯滤波)
- [4-27](#4-27)
  - [改进的canny算子:将高斯滤波改为中值滤波并自适应canny的阈值以及中值滤波的核大小](#改进的canny算子将高斯滤波改为中值滤波并自适应canny的阈值以及中值滤波的核大小)

测试中值滤波 均值滤波以及高斯滤波对canny算子边缘检测的影响    

## 原图

![](img/origin.jpg)  

## 无滤波

![](img/canny.jpg)  

## 均值滤波

![](img/blur.jpg)    

## 中值滤波

![](img/mid.jpg)

## 高斯滤波

![](img/gauss.jpg)

经过测试均值滤波与高斯滤波均会损失边缘信息, 所以最终选择中值滤波作为选项

# 4-27  

## 改进的canny算子:将高斯滤波改为中值滤波并自适应canny的阈值以及中值滤波的核大小