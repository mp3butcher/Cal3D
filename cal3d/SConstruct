env = Environment()
if env['CXX'] == 'g++':
    env.Append(CXXFLAGS = ['-Wall'])

Export('env')
SConscript(dirs=['src/cal3d', 'tools/converter'])
