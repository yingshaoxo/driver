# driver

### 中国工程机器人大赛, 越野小车项目开源

#### 这个比赛最坑的地方在于: 官方给的传感器是垃圾，需要调阀值，但不到最终场地，你无法得到准确的值(就算真的到了，也只有5分钟，那还是你比赛的时间)

#### 另外，该比赛所有核心器件都是抄袭的国外开源，接着搞垄断，高价售卖

##### 最后，奉劝所有专业的程序员，如果硬件构架能力不够强，就不要趟这趟浑水

```
1. 小车无法走直线
2. 小车机动性不足(速度不够快，转弯慢或吃力
3. 小车体积大，重心高
4. 轮子与上坡面之间没有充分的接触，会打滑
5. 不会用360度旋转器(舵机)，得不到稳定的(低速大力)动力支持
```

##### 软件技术分析

这里的代码，由于只用到了传感器，阈值显得非常重要，因为`检测黑线`以及`碰到全黑前进`很重要，所以实际比赛把模拟值调高`30%到50%`是比较稳妥的 （可惜比赛时我忘了）

##### 人事策略分析

如果这个项目不是你一个人做的

务必把所有参赛策略都文字化，因为要协调各组员间的利益(达成一致(妥协))

同时，这也是为了减少由于 沟通不充分、个人一意孤行 所导致的错误
