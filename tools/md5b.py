import os
import argparse
import sys
import struct
import json


class Vector2:
    x = 0.0
    y = 0.0

    def __init__(self):
        pass


class Vector3:
    x = 0.0
    y = 0.0
    z = 0.0

    def __init__(self):
        pass


class Quaternion:
    x = 0.0
    y = 0.0
    z = 0.0
    w = 0.0

    def __init__(self):
        pass


class Model:
    class Bone:
        name = ''
        parent_index = 0
        position = Vector3()
        orientation = Quaternion()

        def __init__(self):
            pass

    class Mesh:
        class Vertex:
            texcoord = Vector2()
            weight_index_start = 0
            weight_count = 0

            def __init__(self):
                pass

        class Weight:
            bone_index = 0
            bias = 0.0
            position = Vector3()

            def __init__(self):
                pass

        material = ''
        vertices = list()
        indices = list()
        weights = list()

        def __init__(self):
            pass

    bones = list()
    meshes = list()

    def __init__(self, path):
        with open(path, 'rb') as f:
            lines = iter(f.read().splitlines())

            for line in lines:
                if len(line) == 0:
                    continue

                tokens = line.split()

                if tokens[0] == 'joints':
                    while 1:
                        line = next(lines)

                        if len(line) == 0:
                            continue
                        elif line == '}':
                            break

                        tokens = line.split()

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
                    while 1:
                        line = next(lines)

                        if len(line) == 0:
                            continue
                        elif line == '}':
                            break

                        tokens = line.split()

                        mesh = Model.Mesh()

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

                    print mesh.material
                    print len(mesh.vertices)
                    print len(mesh.indices)
                    print len(mesh.weights)

                    self.meshes.append(mesh)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', dest='input', required=False, default=os.getcwd())
    parser.add_argument('--output', dest='output', required=False, default=os.getcwd())
    args = parser.parse_args()

    args.input = 'C:\Users\Colin Basnett\Documents\Visual Studio 2013\Projects\mandala\\resources\\raw\models\space_station_4\space_station_4.md5mesh'
    args.output = 'C:/Users/Colin Basnett/Desktop/model.md5b'

    if not os.path.isfile(args.input):
        return 1

    extension = args.input.split('.')[-1].lower()

    if extension == 'md5mesh':
        model = Model(args.input)
        version = 1

        with open(args.output, 'wb+') as f:
            # header
            s = struct.Struct('4s I')
            f.write(s.pack('MD5M', version))

            # bone count
            s = struct.Struct('I')
            f.write(s.pack(len(model.bones)))

            # bones
            for bone in model.bones:
                f.write(bone.name + '\0')

                s = struct.Struct('B f f f f f f')
                f.write(s.pack(bone.parent_index,
                               bone.position.x,
                               bone.position.y,
                               bone.position.z,
                               bone.orientation.x,
                               bone.orientation.y,
                               bone.orientation.z))

            # mesh count
            s = struct.Struct('I')
            f.write(s.pack(len(model.meshes)))

            # meshes
            for mesh in model.meshes:
                # material
                f.write(mesh.material + '\0')

                # vertex count
                s = struct.Struct('I')
                f.write(s.pack(len(mesh.vertices)))

                # vertices
                for vertex in mesh.vertices:
                    s = struct.Struct('f f H B')
                    f.write(s.pack(vertex.texcoord.x,
                                   vertex.texcoord.y,
                                   vertex.weight_index_start,
                                   vertex.weight_count))

                # index count
                s = struct.Struct('I')
                f.write(s.pack(len(mesh.indices)))

                # indices
                for i in range(0, len(mesh.indices) / 3):
                    s = struct.Struct('H H H')
                    f.write(s.pack(mesh.indices[(i * 3) + 0],
                                   mesh.indices[(i * 3) + 1],
                                   mesh.indices[(i * 3) + 2]))

                # weight count
                s = struct.Struct('I')
                f.write(s.pack(len(mesh.weights)))

                for weight in mesh.weights:
                    s = struct.Struct('B f f f f')
                    f.write(s.pack(weight.bone_index,
                                   weight.bias,
                                   weight.position.x,
                                   weight.position.y,
                                   weight.position.z))

    elif extension == 'md5anim':
        pass


if __name__ == '__main__':
    main()