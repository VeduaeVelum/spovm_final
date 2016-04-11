CPP = g++

CFLAGS = -c -Wall -pedantic

CSPECFLAGS = -c -fno-access-control  -fcheck-new -ffriend-injection -fno-elide-constructors -fno-enforce-eh-specs -ffor-scope  -fno-for-scope  -fno-gnu-keywords -fno-implicit-templates -fno-implicit-inline-templates -fno-implement-inlines  -fms-extensions -fno-nonansi-builtins  -fnothrow-opt  -fno-operator-names -fno-optional-diags  -fpermissive -fno-pretty-templates -fno-rtti -fno-threadsafe-statics  -fuse-cxa-atexit -fvisibility-inlines-hidden -fvisibility-ms-compat -fext-numeric-literals -Wabi-tag  -Wconversion-null  -Wctor-dtor-privacy -Wdelete-non-virtual-dtor -Wliteral-suffix -Wnarrowing -Wnoexcept -Wnon-virtual-dtor  -Wreorder -Weffc++  -Wstrict-null-sentinel -Wno-non-template-friend  -Wold-style-cast -Woverloaded-virtual  -Wno-pmf-conversions -Wsign-promo

all: parentFile childFile

parentFile: parentFile.o
	$(CPP) parentFile.o -o par

childFile: childFile.o
	$(CPP) childFile.o -o ch	

parentFile.o: parentFile.cpp
	$(CPP) $(CFLAGS) parentFile.h mySigProcessor.h parentFile.cpp

childFile.o: childFile.cpp
	$(CPP) $(CFLAGS) childFile.h childFile.cpp

clean:
	rm -rf *.o par
	rm -rf *.o ch
	rm -rf *h.gch
	rm -rf ch
	rm -rf par