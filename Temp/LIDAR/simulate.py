import matplotlib.pyplot as plt
import numpy as np

def perp(a):
    return np.array([-a[1], a[0]])

def intersect(a, b):
    da = a[1] - a[0]
    db = b[1] - b[0]
    dp = a[0] - b[0]
    dap = perp(da)
    dapdp = np.dot(dap, dp)
    dapdb = np.dot(dap, db)
    if (dapdb == 0): return None
    r = (dapdp / dapdb) * db + b[0]
    if ((r[0] < max(min(a[0][0], a[1][0]), min(b[0][0], b[1][0]))) or (r[0] > min(max(a[0][0],a[1][0]), max(b[0][0],b[1][0])))): return None
    if ((r[1] < max(min(a[0][1], a[1][1]), min(b[0][1], b[1][1]))) or (r[1] > min(max(a[0][1],a[1][1]), max(b[0][1],b[1][1])))): return None
    return r

def cart2pol(t):
    x, y = t
    return np.array([np.arctan2(y, x), np.sqrt(x**2 + y**2)])

def pol2cart(t, o):
    th, r = t
    return np.array([r * np.cos(th) + o[0], r * np.sin(th) + o[1]])

if __name__ == "__main__":
    O = np.array([            0,    0])
    Q = np.array([            0,  400])
    A = np.array([2 * np.pi / 3, 1000])
    B = np.array([4 * np.pi / 3, 1000])
    C = np.array([7 * np.pi / 4, 2000])
    D = np.array([1 * np.pi / 4, 2000])

    Qr = pol2cart(Q, O)
    Ar = pol2cart(A, Qr)
    Br = pol2cart(B, Qr)
    Cr = pol2cart(C, Qr)
    Dr = pol2cart(D, Qr)

    AB = np.array([Ar, Br])
    BC = np.array([Br, Cr])
    CD = np.array([Cr, Dr])
    DA = np.array([Dr, Ar])

    sides = [AB, BC, CD, DA]

    th = np.zeros(360)
    r = np.zeros(360, 'int')
    for i in xrange(360):
        M = np.array([i * np.pi / 180, 2000])
        Mr = pol2cart(M, O)
        OM = np.array([O, Mr])
        for j in xrange(4):
            Ir = intersect(OM, sides[j])
            if Ir is not None:
                I = cart2pol(Ir)
                if I[1] > 2000: I[1] = 0
                th[i], r[i] = I
                break
                
    r.tofile("r.csv", sep=',', format="%4d")
    print th[359], r[359]
    plt.figure("Raw Data")
    ax = plt.subplot(111, polar=True)
    ax.grid(True)
    ax.plot((A[0], B[0]), (A[1], B[1]))
    ax.plot((B[0], C[0]), (B[1], C[1]))
    ax.plot((C[0], D[0]), (C[1], D[1]))
    ax.plot((D[0], A[0]), (D[1], A[1]))
    ax.plot((O[0], M[0]), (O[1], M[1]))
    ax.scatter(O[0], O[1], 200, 'red')
    ax.scatter(Q[0], Q[1], 200, 'green')
    ax.scatter(th, r)
    ax.set_rmax(2200)
    plt.show()
