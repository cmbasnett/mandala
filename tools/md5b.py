import os
import argparse
import sys
import struct
import json


def iequal(a, b):
    try:
        return a.upper() == b.upper()
    except AttributeError:
        return a == b


class Vector2:
    def __init__(self):
        self.x = 0.0
        self.y = 0.0


class Vector3:
    def __init__(self):
        self.x = 0.0
        self.y = 0.0
        self.z = 0.0


class Quaternion:
    def __init__(self):
        self.x = 0.0
        self.y = 0.0
        self.z = 0.0
        self.w = 0.0
        

class Animation:
    class Bone:
        def __init__(self):
            self.name = ''
            self.parent_index = 0
            self.flags = 0
            self.frame_data_start_index = 0

    class BoneFrame:
        def __init__(self):
            self.position = Vector3()
            self.orientation = Quaternion()

    class Frame:
        def __init__(self):
            self.min = Vector3()
            self.max = Vector3()
            self.data = list()

    def __init__(self, path):
        self.base_bone_frames = list()
        self.frames = list()
        self.bones = list()
        self.frames_per_second = 0
        self.frame_data_count = 0

        with open(path, 'rb') as f:
            lines = iter(list(line for line in (l.strip() for l in f) if line))

            for line in lines:
                tokens = line.split()

                if iequal(tokens[0], 'md5version') and tokens[1] != '10':
                    raise Exception('invalid version')
                elif iequal(tokens[0], 'framerate'):
                    self.frames_per_second = int(tokens[1])
                elif iequal(tokens[0], 'numanimatedcomponents'):
                    self.frame_data_count = int(tokens[1])
                elif iequal(tokens[0], 'hierarchy'):
                    while True:
                        tokens = next(lines).split()

                        if tokens[0] == '}':
                            break

                        bone = Animation.Bone()
                        bone.name = tokens[0].strip('"')

                        bone.parent_index = int(tokens[1])

                        if bone.parent_index == -1:
                            bone.parent_index = 255

                        bone.flags = int(tokens[2])
                        bone.frame_data_start_index = int(tokens[3])

                        self.bones.append(bone)
                elif iequal(tokens[0], 'bounds'):
                    while True:
                        tokens = next(lines).split()

                        if tokens[0] == '}':
                            break

                        frame = Animation.Frame()
                        frame.min.x = float(tokens[1])
                        frame.min.y = float(tokens[2])
                        frame.min.z = float(tokens[3])
                        frame.max.x = float(tokens[6])
                        frame.max.y = float(tokens[7])
                        frame.max.z = float(tokens[8])

                        self.frames.append(frame)
                elif iequal(tokens[0], 'baseframe'):
                    while True:
                        tokens = next(lines).split()

                        if tokens[0] == '}':
                            break

                        bone_frame = Animation.BoneFrame()
                        bone_frame.position.x = float(tokens[1])
                        bone_frame.position.y = float(tokens[2])
                        bone_frame.position.z = float(tokens[3])
                        bone_frame.orientation.x = float(tokens[6])
                        bone_frame.orientation.y = float(tokens[7])
                        bone_frame.orientation.z = float(tokens[8])

                        self.base_bone_frames.append(bone_frame)
                elif iequal(tokens[0], 'frame'):
                    frame_index = int(tokens[1])

                    while True:
                        tokens = next(lines).split()

                        if tokens[0] == '}':
                            break

                        for token in tokens:
                            self.frames[frame_index].data.append(float(token))

    def write(self, path):
        version = 1

        with open(path, 'wb+') as f:
            # todo: make this a function of model somehow

            # header
            s = struct.Struct('4sII')
            f.write(s.pack('MD5A',
                           version,
                           self.frames_per_second))

            # bone count
            s = struct.Struct('I')
            f.write(s.pack(len(self.bones)))

            # bones
            for bone in self.bones:
                f.write(bone.name + '\0')

                s = struct.Struct('B')
                f.write(s.pack(bone.parent_index))
                f.write(s.pack(bone.flags))

                s = struct.Struct('i')
                f.write(s.pack(bone.frame_data_start_index))

            # frame count
            s = struct.Struct('I')
            f.write(s.pack(len(self.frames)))

            # frame bounds
            for frame in self.frames:
                s = struct.Struct('ffffff')
                f.write(s.pack(frame.min.x,
                               frame.min.y,
                               frame.min.z,
                               frame.max.x,
                               frame.max.y,
                               frame.max.z))

            # base bone frames
            for bone_frame in self.base_bone_frames:
                s = struct.Struct('ffffff')
                f.write(s.pack(bone_frame.position.x,
                               bone_frame.position.y,
                               bone_frame.position.z,
                               bone_frame.orientation.x,
                               bone_frame.orientation.y,
                               bone_frame.orientation.z))

            # frame data count
            s = struct.Struct('I')
            f.write(s.pack(self.frame_data_count))

            # frame data
            s = struct.Struct('f')
            for frame in self.frames:
                for data in frame.data:
                    f.write(s.pack(data))


class Model:
    class Bone:
        def __init__(self):
            self.name = ''
            self.parent_index = 0
            self.position = Vector3()
            self.orientation = Quaternion()

    class Mesh:
        class Vertex:
            def __init__(self):
                self.texcoord = Vector2()
                self.weight_index_start = 0
                self.weight_count = 0

        class Weight:
            def __init__(self):
                self.bone_index = 0
                self.bias = 0.0
                self.position = Vector3()

        def __init__(self):
            self.material = ''
            self.vertices = list()
            self.indices = list()
            self.weights = list()

    def __init__(self, path):
        self.bones = list()
        self.meshes = list()

        with open(path, 'rb') as f:
            lines = iter(list(line for line in (l.strip() for l in f) if line))

            for line in lines:
                tokens = line.split()

                if tokens[0] == 'joints':
                    while True:
                        tokens = next(lines).split()

                        if tokens[0] == '}':
                            break

                        bone = Model.Bone()
                        bone.name = tokens[0].strip('"')
                        bone.parent_index = int(tokens[1])
                        bone.position.x = float(tokens[3])
                        bone.position.y = float(tokens[4])
                        bone.position.z = float(tokens[5])
                        bone.orientation.x = float(tokens[8])
                        bone.orientation.y = float(tokens[9])
                        bone.orientation.z = float(tokens[10])

                        if bone.parent_index == -1:
                            bone.parent_index = 255

                        self.bones.append(bone)
                elif tokens[0] == 'mesh':
                    mesh = Model.Mesh()
                    while True:
                        tokens = next(lines).split()

                        if len(tokens) == 0:
                            continue
                        elif tokens[0] == '}':
                            break

                        if tokens[0] == 'shader':
                            mesh.material = tokens[1].strip('"')
                        elif tokens[0] == 'vert':
                            vertex = Model.Mesh.Vertex()
                            vertex.texcoord.x = float(tokens[3])
                            vertex.texcoord.y = float(tokens[4])
                            vertex.weight_index_start = int(tokens[6])
                            vertex.weight_count = int(tokens[7])

                            mesh.vertices.append(vertex)
                        elif tokens[0] == 'tri':
                            mesh.indices.append(int(tokens[2]))
                            mesh.indices.append(int(tokens[3]))
                            mesh.indices.append(int(tokens[4]))
                        elif tokens[0] == 'weight':
                            weight = Model.Mesh.Weight()
                            weight.bone_index = int(tokens[2])
                            weight.bias = float(tokens[3])
                            weight.position.x = float(tokens[5])
                            weight.position.y = float(tokens[6])
                            weight.position.z = float(tokens[7])

                            mesh.weights.append(weight)

                    self.meshes.append(mesh)

    def write(self, path):
        version = 1

        with open(path, 'wb+') as f:
            # header
            s = struct.Struct('4s I')
            f.write(s.pack('MD5M', version))

            # bone count
            s = struct.Struct('I')
            f.write(s.pack(len(self.bones)))

            # bones
            for bone in self.bones:
                f.write(bone.name + '\0')

                s = struct.Struct('B')
                f.write(s.pack(bone.parent_index))

                s = struct.Struct('ffffff')
                f.write(s.pack(bone.position.x,
                               bone.position.y,
                               bone.position.z,
                               bone.orientation.x,
                               bone.orientation.y,
                               bone.orientation.z))

            s = struct.Struct('B')
            f.write(s.pack(len(self.meshes)))

            # meshes
            for mesh in self.meshes:
                # material
                f.write(mesh.material + '\0')

                # vertex count
                s = struct.Struct('I')
                f.write(s.pack(len(mesh.vertices)))

                # vertices
                for vertex in mesh.vertices:
                    s = struct.Struct('ffHB')
                    f.write(s.pack(vertex.texcoord.x,
                                   vertex.texcoord.y,
                                   vertex.weight_index_start,
                                   vertex.weight_count))

                # index count
                s = struct.Struct('I')
                f.write(s.pack(len(mesh.indices)))

                # indices
                for i in range(0, len(mesh.indices) / 3):
                    s = struct.Struct('HHH')
                    f.write(s.pack(mesh.indices[(i * 3) + 0],
                                   mesh.indices[(i * 3) + 1],
                                   mesh.indices[(i * 3) + 2]))

                # weight count
                s = struct.Struct('I')
                f.write(s.pack(len(mesh.weights)))

                for weight in mesh.weights:
                    s = struct.Struct('B')
                    f.write(s.pack(weight.bone_index))
                    s = struct.Struct('ffff')
                    f.write(s.pack(weight.bias,
                                   weight.position.x,
                                   weight.position.y,
                                   weight.position.z))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-q', '--quiet', dest='quiet', action='store_true', help='no output')
    parser.add_argument('input_file', help='path to [.md5anim] or [.md5mesh] file')
    parser.add_argument('-o', dest='output_dir', help='path to directory where [.md5a] or [.md5m] file will be written', default=os.getcwd())
    args = parser.parse_args()

    if not os.path.isfile(args.input_file):
        raise RuntimeError('input_file {} does not exist'.format(args.input_file))

    #TODO: verify beforehand that output_file will be created properly/create it here and pass it
    output_file = os.path.join(args.output_dir, os.path.splitext(os.path.basename(args.input_file))[0])
    extension = os.path.splitext(args.input_file)[1]

    if iequal(extension, '.md5mesh'):
        model = Model(args.input_file)
        model.write(output_file + '.md5m')
    elif iequal(extension, '.md5anim'):
        animation = Animation(args.input_file)
        animation.write(output_file + '.md5a')
    else:
        raise RuntimeError('invalid extension')


if __name__ == '__main__':
    main()