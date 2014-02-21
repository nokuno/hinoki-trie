def options(opt):
    opt.load('compiler_cxx unittest_gtest')

def configure(conf):
    conf.load('compiler_cxx unittest_gtest')
    conf.env.CXXFLAGS += ['-O3', '-Wall', '-std=c++0x']

def build(bld):
    bld.program(source = 'build.cpp', target = 'hinoki-build')
    bld.program(source = 'lookup.cpp', target = 'hinoki-lookup')
    bld.program(source = 'predict.cpp', target = 'hinoki-predict')
    bld.program(source = 'correct.cpp', target = 'hinoki-correct')
    bld.program(source = 'traverse.cpp', target = 'hinoki-traverse')
    bld.program(source = bld.path.ant_glob('*Test.cpp'), target = 'test',
            features = 'testt', lib=['gtest','gtest_main','pthread'])
