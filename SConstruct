# Target Compilation Mode #
base_compilation_flags = []
additional_flags = []
possible_targets = ['dev', 'stable']
target = 'dev'

# Example usage: scons target=dev,stable #
if 'target' in ARGUMENTS:
    target = str(ARGUMENTS['target']).lower()
if not target in possible_targets:
    target = 'dev'

base_compilation_flags.append('-DTARGET=' + target.upper())
bin_target = 'bin/' + target + '/'
build_target = 'build/' + target + '/'

# Enviroment Setup #
env = Environment(
    COMPILATIONDB_USE_ABSPATH=True,
    CPPPATH=['#/include', '#/include/crystal', '#/include/vendor', '#/include/funkin'],
    SCONS_CXX_STANDARD='cxx17',
    CCFLAGS=base_compilation_flags
)

platform = env['PLATFORM']

match platform:
    case 'win32': # Windows
        match target:
            case 'stable':
                additional_flags = ['/O2', '/MT', '/EHsc']
            case 'dev':
                additional_flags =['/DEBUG', '/Od', '/MTd', '/EHsc']
        env.Append(
            CCFLAGS = additional_flags,
            LINKFLAGS = ['/NODEFAULTLIB:library', '/LTCG'],
            LIBPATH = ['#/lib/win64'],
            LIBS = ['libpng16', 'glfw3dll', 'OpenAL32', 'freetype', 'zlib'],
        )
    case _: # Linux (not sure the actual value for linux atm)
        match target:
            case 'stable':
                additional_flags = ['-O3', '-Wall']
            case 'dev':
                additional_flags =['-g', '-O0', '-Wall']
        env.Append(
            CCFLAGS = additional_flags,
            LIBPATH=['#/lib'],
            LIBS = ['glfw', 'openal', 'freetype', 'zlib', 'png'],
        )

# Compatibility with the Sublime Text plugin I use for CPP Autocomplete #
# Example usage: scons no_compile_commands=1 #
if not 'no_compile_commands' in ARGUMENTS:
    env.Tool('compilation_db')
    env.CompilationDatabase()

# Exports #

Export('env')
Export('build_target')

# TODO: Convert the objects to libraries to make shit work on windows
# ;-;

# Compilation #
SConscript('src/SConscript', exports='env', variant_dir='#/' + build_target, duplicate=0)
env.Program(target=bin_target + 'Funkin', source=Glob('#/' + build_target + '**.o**'))

# Assets #

# we use shutil because scons itself was hard to get working this specific way, sorry scons devs!
import shutil
shutil.copytree(src = 'assets', dst = bin_target + 'assets', dirs_exist_ok = True)
