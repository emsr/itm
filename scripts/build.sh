#! /bin/bash

g++ -I include -g -shared -o libITModel.so src/*.cpp
g++ -I include -g -o RunITModel RunITModel.cpp -L. -lITModel

LD_LIBRARY_PATH=. ./RunITModel < test/input/itm_1.in > test/output/itm_1.out

LD_LIBRARY_PATH=. ./RunITModel 1 < test/input/test1.in > test/output/test1.out
LD_LIBRARY_PATH=. ./RunITModel 1 < test/input/test2.in > test/output/test2.out
LD_LIBRARY_PATH=. ./RunITModel 1 < test/input/test3.in > test/output/test3.out
LD_LIBRARY_PATH=. ./RunITModel 1 < test/input/test4.in > test/output/test4.out
LD_LIBRARY_PATH=. ./RunITModel 1 < test/input/test5.in > test/output/test5.out

# These work like a charm (check cmd_examples/ outputs):
LD_LIBRARY_PATH=. ./RunITModel < test/input/p2p0.in > test/output/p2p0.out
LD_LIBRARY_PATH=. ./RunITModel < test/input/p2p1.in > test/output/p2p1.out
LD_LIBRARY_PATH=. ./RunITModel < test/input/p2p2.in > test/output/p2p2.out
LD_LIBRARY_PATH=. ./RunITModel < test/input/p2p3.in > test/output/p2p3.out

RunTIREModel < test/input/tirem_1.in > test/output/tiremodel.out
../../work/tirem3/build/tirem64/RunTiremAPI < test/input/tirem_1.in > test/output/tirem_1.out

LD_LIBRARY_PATH=../LongleyRice ../LongleyRice/RunLongleyRice  < test/input/itm_1.in > test/output/lr_1.out
