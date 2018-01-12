# -*-coding:utf-8-*-
import matplotlib.pyplot as plt

from random_walk import RandomWalk

print("hello")
# 创建一个RandomWalk实例，并将其包含的点都绘制出来

rw = RandomWalk(5000)
rw.fill_walk()

# 设置绘图窗口的尺寸
plt.figure(dpi=128,figsize=(10,6))

# 让点的颜色随着生成顺序加深
point_number = list(range(rw.num_points))
plt.scatter(rw.x_values, rw.y_values, c=point_number, cmap=plt.cm.Reds, s = 15)

# 突出起点和终点，起点用绿色表示，终点用红色表示
plt.scatter(0, 0, c='green', edgecolors='none', s=100)
plt.scatter(rw.x_values[-1], rw.y_values[-1], c='red', edgecolors='none', s=100)

# 隐藏坐标轴
plt.axes().get_xaxis().set_visible(False)
plt.axes().get_yaxis().set_visible(False)

plt.show()
