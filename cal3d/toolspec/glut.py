def generate(env):
    env.Append(LIBS=['glut32', 'opengl32', 'glu32'])

def exists(env):
    return 1
