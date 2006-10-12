import os
execfile(os.path.join(os.path.dirname(__file__), '../../Dll/main/prologue.py'))

import sys
from pycal3d import Cal3d


def usage():
    print 'Usage: cal3d_convert.py <input> <output>'
    return 2


def main(argv=sys.argv):
    if len(argv) != 3:
        return usage()
    c_input  = argv[1]
    c_output = argv[2]

    data = open(c_input, 'rb').read()

    data = Cal3d().convertToBinary("CoreAnimation", data)

    open(c_output, 'wb').write(data)


if __name__ == '__main__':
    sys.exit(main(sys.argv) or 0)
