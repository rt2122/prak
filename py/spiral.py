def gen():
    i = 0;
    while True:
        yield i
        i = (i + 1) % 10

import numpy as np
#m, n = eval(input())
a = np.full((6, 5), 10)
a[3][2] = 8
print(a)
'''
i, j, direc = 0, 0, "right"
w = 50
for x in gen():
    print(i, j, direc)
    w -= 1
    if w == 0:
        break
    a[i][j] = x
    if direc == "right":
        i += 1
        if i == m or a[i][j] != 10:
            direc = "down"
            i -= 1
            j += 1
    elif direc == "down":
        j += 1
        if j == n or a[i][j] != 10:
            direc = "left"
            i -= 1
            j -= 1
    elif direc == "left":
        i -= 1
        if i < 0 or a[i][j] != 10:
            direc = "up"
            i += 1
            j -= 1
    else:
        j -= 1
        if a[i][j] != 0:
            direc = "right"
            i += 1
            j -= 1
    if a[i][j] != 10:
        break;

print(a)
'''