import math
a = eval(input())
a = sorted(a, key = lambda x: math.sin(float(x)))
print(a)