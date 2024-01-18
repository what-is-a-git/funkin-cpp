# Target Compilation Mode #
base_compilation_flags = ['-Wall']
additional_flags = []
possible_targets = ['dev', 'stable']
target = 'dev'

# Example usage: scons target=dev,stable #
if 'target' in ARGUMENTS:
    target = str(ARGUMENTS['target']).lower()
if not target in possible_targets:
    target = 'dev'

match target:
    case 'stable':
        additional_flags = ['-O3']
    case 'dev':
        additional_flags =['-g', '-O0']

base_compilation_flags.append('-DTARGET=' + target.upper())
bin_target = 'bin/' + target + '/'
build_target = 'build/' + target + '/'

# Enviroment Setup #
env = Environment(
    COMPILATIONDB_USE_ABSPATH=True,
    LIBPATH=['#/lib'],
    LIBS=['glfw', 'openal', 'freetype'],
    CPPPATH=['#/include', '#/include/crystal', '#/include/vendor', '#/include/funkin'],
    SCONS_CXX_STANDARD='cxx17',
    CCFLAGS=base_compilation_flags + additional_flags
)

# Compatibility with the Sublime Text plugin I use for CPP Autocomplete #
# Example usage: scons no_compile_commands=1 #
if not 'no_compile_commands' in ARGUMENTS:
    env.Tool('compilation_db')
    env.CompilationDatabase()

# Exports #

Export('env')
Export('build_target')

# Compilation #
SConscript('src/SConscript', exports='env', variant_dir='#/' + build_target, duplicate=0)
env.Program(target=bin_target + 'Funkin', source=Glob('#/' + build_target + '**.o'))

# Assets #

# we use shutil because scons itself was hard to get working this specific way, sorry scons devs!
import shutil
shutil.copytree(src = 'assets', dst = bin_target + 'assets', dirs_exist_ok = True)
