env = Environment()
if env['CXX'] == 'g++':
    env.Append(CXXFLAGS = ['-Wall'])

Export('env')
SConscript('src/cal3d/SConscript')
