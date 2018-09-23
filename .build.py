#!/usr/bin/env python3


import argparse
import os
import sys

parser = argparse.ArgumentParser()
parser.add_argument('--compiler',
                    type=str,
                    help='the compiler used',
                    nargs='?',
                    default='gcc',
                    const='gcc')
parser.add_argument('--cxx',
                    type=int,
                    help='the ISO C++ standard',
                    nargs='?',
                    default=17,
                    const=17)
parser.add_argument('--build_type',
                    type=str,
                    help='',
                    nargs='?',
                    default='debug',
                    const='debug')


def main():
    args = parser.parse_args()

    buildPath = ''
    buildPath += os.path.realpath(sys.path[0])
    buildPath += '/build/'
    buildPath += args.compiler
    buildPath += '/cxx'
    buildPath += str(args.cxx)
    buildPath += '/'
    buildPath += args.build_type

    if not os.path.isdir(buildPath):
        os.makedirs(buildPath)

    command = '('
    command += 'export CC='
    command += args.compiler
    command += '; export CXX='

    if args.compiler.lower() == 'gcc':
        command += 'g++'
    elif args.compiler.lower() == 'clang':
        command += 'clang++'

    command += '; cd '
    command += buildPath
    command += '; cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE='
    command += args.build_type.lower().capitalize()
    command += ' -DEXAMPLES:BOOL=ON '
    command += os.path.realpath(sys.path[0])
    command += '; make -j4; cd '
    command += os.path.realpath(sys.path[0])
    command += ')'

    print(buildPath)
    print(command)

    os.system(command)


if __name__ == "__main__":
    main()
