# -*-coding:utf-8-*-
import matplotlib.pyplot as plt

x_values = list(range(1,1001))
y_values = [x**2 for x in x_values]

# s表示线的宽度,edgecolor表示数据点的轮廓,c表示数据点的颜色
# pyplot内置了一组颜色映射，利用y设置其颜色，并使用参数cmap告诉pyplot使用哪个颜色做映射
plt.scatter(x_values, y_values, c=y_values, cmap=plt.cm.Blues, edgecolor='none', s=5)

# 设置图表标题，并给坐标轴加上标签
plt.title("Squares Numbers", fontsize=24)
plt.xlabel("Value", fontsize=14)
plt.ylabel("Square of Value", fontsize=14)

# 设置刻度标记的大小
plt.tick_params(axis='both', labelsize=14)

plt.axis([0, 1100, 0, 1100000])

# 第一个参数为图表的名称以及类型，第二个实参指定将图表多余的空白区域裁剪掉
plt.savefig('square_plot.png',bbox_inches='tight')