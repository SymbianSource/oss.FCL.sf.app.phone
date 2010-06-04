ut_parserrecognizer

If you run qmake then please modify the created mmp file of this project.
You need to remove \epoc32\include from the mmp. Otherwise the parserrecognizer.cpp
will see the original instead of the mocked version.
