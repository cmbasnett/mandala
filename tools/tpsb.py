import os
import argparse
import json
import struct


def main():
    MAGIC = 'TPSB'
    VERSION = 1

    parser = argparse.ArgumentParser()
    #parser.add_argument('json_file', help='json file to be processed')
    parser.add_argument('-q', '--quiet', dest='quiet', required=False, action='store_true', help='no output')
    args = parser.parse_args()

    data = None

    args.json_file = 'C:\Users\Colin Basnett\Documents\Visual Studio 2013\Projects\\testapp\\resources_src\sprite_sets\crosshairs\crosshairs.json'

    with open(args.json_file) as f:
        data = json.loads(f.read())

    filename, extension = os.path.splitext(os.path.basename(args.json_file))

    with open('{}.tpsb'.format(filename), 'wb') as f:
        f.write(MAGIC)
        f.write(struct.pack('I', VERSION))
        f.write(data['meta']['image'])
        f.write(struct.pack('c', '\0'))
        f.write(struct.pack('H', len(data['frames'])))

        texture_size = data['meta']['size']

        for region in data['frames']:
            frame_rectangle = region['frame']
            source_size = region['sourceSize']
            rectangle = region['spriteSourceSize']
            flags = 0

            if region['rotated']:
                flags |= (1 << 0)

            if region['trimmed']:
                flags |= (1 << 1)

            original_frame_rectangle = frame_rectangle

            if region['rotated']:
                frame_rectangle['w'], frame_rectangle['h'] = frame_rectangle['h'], frame_rectangle['w']

            f.write(region['filename'])
            f.write(struct.pack('c', '\0'))
            f.write(struct.pack('hhhh', frame_rectangle['x'], frame_rectangle['y'], frame_rectangle['w'], frame_rectangle['h']))
            f.write(struct.pack('hhhh', rectangle['x'], source_size['h'] - rectangle['y'] - original_frame_rectangle['h'], rectangle['w'], rectangle['h']))
            f.write(struct.pack('hh', source_size['w'], source_size['h']))

            f.write(struct.pack('f', float(frame_rectangle['x']) / texture_size['w']))
            f.write(struct.pack('f', float(texture_size['h'] - frame_rectangle['y'] - frame_rectangle['h']) / texture_size['h']))
            f.write(struct.pack('f', float(frame_rectangle['x'] + frame_rectangle['w']) / texture_size['w']))
            f.write(struct.pack('f', float(texture_size['h'] - frame_rectangle['y']) / texture_size['h']))

            f.write(struct.pack('B', flags))

if __name__ == '__main__':
    main()
