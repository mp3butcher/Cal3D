import sys

env = Environment()

# Since we can't use toolpath=['#/toolspec'] yet...
sys.path.append(Dir('#/toolspec').abspath)

optCache = 'options.cache'
opts = Options(optCache)
opts.AddOptions(
    BoolOption('dbg',    'Build with debugging enabled', 0),
    BoolOption('opt',    'Build with optimizations', 0),
    BoolOption('distcc', 'Enable compiling with distcc', 0),
    BoolOption('nowarn', 'Disable all warnings', 0),
    BoolOption('warningsAsErrors', 'Treat warnings as errors', 0))

opts.Update(env)
Help(opts.GenerateHelpText(env))
opts.Save(optCache, env)

if env.get('dbg'):
    env.Append(CCFLAGS=['-g'], CPPDEFINES=['DEBUG', '_DEBUG'])
if env.get('opt'):
    env.Append(CCFLAGS=['-O2'], LINKFLAGS=['-s'], CPPDEFINES=['NDEBUG'])
if env.get('distcc'):
    env['CC']  = ['$(', 'distcc', '$)', env['CC']]
    env['CXX'] = ['$(', 'distcc', '$)', env['CXX']]
if env.get('nowarn'):
    env.Append(CCFLAGS=['-w'])
else:
    if env['CC'] == 'gcc':
        env.Append(CCFLAGS=['-Wall'])
    elif env['CXX'] == 'g++':
        env.Append(CXXFLAGS=['-Wall'])
    if env['CXX'] == 'g++':
        env.Append(CXXFLAGS=['-Wno-non-virtual-dtor'])
if env.get('warningsAsErrors'):
    if env['CC'] == 'gcc':
        env.Append(CCFLAGS=['-Werror'])
    elif env['CXX'] == 'g++':
        env.Append(CXXFLAGS=['-Werror'])

Export('env')
SConscript(dirs=['examples', 'src', 'tools'])
