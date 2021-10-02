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
        starLocation[i][0] = Data[i,0]
        starLocation[i][1] = Data[i,1]
        starLocation[i][2] = Data[i,2]
        starVelocity[i][0] = Data[i,3]
        starVelocity[i][1] = Data[i,4]
        starVelocity[i][2] = Data[i,5]
        starMass[i]        = Data[i,6]

@ti.func
def Force(i,j):
    Res = ti.Vector([0.0, 0.0, 0.0], dt=ti.f32)
    if not i == j:
        Dir = starLocation[j] - starLocation[i]
        Dis = Dir.norm()
        Dis = Dis * Dis * Dis
        Res = starMass[j] * Dir / Dis
    return Res

@ti.kernel
def ComputeForce():
    for i in starForce:
        starForce[i] = (0, 0, 0)
        for j in range(ti.static(starCnt)):
            starForce[i] += Force(i,j)

@ti.kernel
def Forward(T: ti.f32):
    for i in starVelocity:
        starVelocity[i] += T * starForce[i]
    for i in starLocation:
        starLocation[i] += T * starVelocity[i]

def Step():
    ComputeForce()
    for i in range(30):
        Forward(0.001)

def Export(i: int):
    npL = starLocation.to_numpy()

    mesh_writer = ti.PLYWriter(num_vertices=starCnt, face_type="quad")
    mesh_writer.add_vertex_pos(npL[:, 0], npL[:, 1], npL[:, 2])

    mesh_writer.export_frame(i, 'S.ply')

    print('%03d'%(i))

def main():
    Init(starData)

    try:
        for i in range(100):
            Step()
            Export(i)
    except Exception as Error:
        print(Error)

if __name__=='__main__':
    main()