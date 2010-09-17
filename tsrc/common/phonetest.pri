######################################################################
# Phone unit test qmake project include 
######################################################################

runtest_target.target = runtest
runtest_target.commands = call "$${EPOCROOT}epoc32/release/winscw/udeb/$${TARGET}.exe" -dtextshell --
QMAKE_EXTRA_TARGETS += runtest_target

