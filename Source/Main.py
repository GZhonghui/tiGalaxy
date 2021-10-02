import taichi as ti
import Loader
import copy

UseConfig = 5

Configs = list()
Config  = dict()

#Scene 0
Config['fileName']    = 'Galaxy20K.bin'
Config['subStep']     = 5
Config['subStepTime'] = 0.003
Config['cameraPos']   = (0, 0, 60)
Config['Radious']     = 0.1
Configs.append(copy.deepcopy(Config))

#Scene 1
Config['fileName']    = 'Dubinski.tab'
Config['subStep']     = 30
Config['subStepTime'] = 0.01
Config['cameraPos']   = (0, 0, 80)
Config['Radious']     = 0.1
Configs.append(copy.deepcopy(Config))

#Scene 2
Config['fileName']    = 'Tab65536.tab'
Config['subStep']     = 30
Config['subStepTime'] = 0.01
Config['cameraPos']   = (0, 0, 60)
Config['Radious']     = 0.1
Configs.append(copy.deepcopy(Config))

#Scene 3
Config['fileName']    = 'Stars.dat'
Config['subStep']     = 30
Config['subStepTime'] = 0.001
Config['cameraPos']   = (0, 0, 200)
Config['Radious']     = 0.1
Configs.append(copy.deepcopy(Config))

#Scene 4
Config['fileName']    = 'K17C.snap'
Config['subStep']     = 30
Config['subStepTime'] = 0.003
Config['cameraPos']   = (0, 0, 20)
Config['Radious']     = 0.03
Configs.append(copy.deepcopy(Config))

#Scene 5
Config['fileName']    = 'K17HP.snap'
Config['subStep']     = 30
Config['subStepTime'] = 0.003
Config['cameraPos']   = (15, 15, 15)
Config['Radious']     = 0.03
Configs.append(copy.deepcopy(Config))

Loader.Load(Configs[UseConfig]['fileName'])
starCnt,starData = Loader.Fill()
Loader.Reset() # Clear Useless Buffer

subStep     = Configs[UseConfig]['subStep']
subStepTime = Configs[UseConfig]['subStepTime']

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
        Res = starMass[j] * Dir / Dis # No Mass
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
        starVelocity[i] += T * starForce[i] # No Mass
    for i in starLocation:
        starLocation[i] += T * starVelocity[i]

def Step():
    ComputeForce()
    for i in range(subStep):
        Forward(subStepTime)

def Export(i: int):
    npL = starLocation.to_numpy()

    mesh_writer = ti.PLYWriter(num_vertices=starCnt, face_type="quad")
    mesh_writer.add_vertex_pos(npL[:, 0], npL[:, 1], npL[:, 2])

    mesh_writer.export_frame_ascii(i, 'S.ply')

    print('%03d'%(i))

def main():
    Init(starData)

    mainWindow = ti.ui.Window("Stars", (1024, 768))
    Canvas = mainWindow.get_canvas()
    Scene = ti.ui.Scene()
    Camera = ti.ui.make_camera()

    Camera.position(*Configs[UseConfig]['cameraPos'])
    Camera.lookat(0, 0, 0)
    Camera.fov(75)

    FrameIndex = 0

    try:
        while mainWindow.running:
            Step()
            
            Scene.set_camera(Camera)
            Scene.ambient_light((0.4, 0.4, 1.0))
            Scene.point_light(pos=( 80,  80,  80), color=(0.4, 0.6, 0.9))
            Scene.point_light(pos=(-80, -80, -80), color=(0.6, 0.5, 0.9))

            Scene.particles(starLocation, radius=Configs[UseConfig]['Radious'], color=(0.5,0.5,1))

            Canvas.scene(Scene)
            mainWindow.show()

            FrameIndex += 1
            if False and not FrameIndex % 30:
                Export(FrameIndex//30)

    except Exception as Error:
        print(Error)

if __name__=='__main__':
    print('Count:',starCnt)
    main()