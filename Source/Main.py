import taichi as ti
import Loader

Loader.Load('Galaxy20K.bin')
starCnt,starData = Loader.Fill()

ti.init(arch=ti.cuda)

starLocation = ti.Vector.field(3, dtype=ti.f32, shape=starCnt)
starVelocity = ti.Vector.field(3, dtype=ti.f32, shape=starCnt)
starForce    = ti.Vector.field(3, dtype=ti.f32, shape=starCnt)
starMass     = ti.field(dtype=ti.f32, shape=starCnt)

@ti.kernel
def Init(Data: ti.ext_arr()):
    for i in range(ti.static(starCnt)):
        starLocation[i][0] = Data[i][0]
        starLocation[i][1] = Data[i][1]
        starLocation[i][2] = Data[i][2]
        starVelocity[i][0] = Data[i][3]
        starVelocity[i][1] = Data[i][4]
        starVelocity[i][2] = Data[i][5]
        starMass[i]        = Data[i][6]

@ti.func
def Force(i,j):
    Res = ti.Vector([0.0, 0.0, 0.0], dt=ti.f32)
    if not i == j:
        pass
    return Res

@ti.kernel
def ComputeForce():
    for i in starForce:
        starForce[i] = (0, 0, 0)
        for j in starLocation:
            starForce += Force(i,j)

@ti.kernel
def Forward(T):
    for i in starVelocity:
        starVelocity[i] += T * starForce[i] / starMass[i]
    for i in starLocation:
        starLocation[i] += T * starVelocity[i]

def Step():
    ComputeForce()
    Move()

def main():
    Init(starData)

    try:
        while True:
            Step()
    except Exception as Error:
        print(Error)

if __name__=='__main__':
    main()