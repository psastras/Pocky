# Paul Sastrasinh

# C/C++ Build variables
CC  = ndk-build
SRCDIR := jni
OBJDIR := obj


# Java build variables
JAVA_CLASSDIR := bin/classes/
JAVA_SOURCES := $(wildcard $(src/pocky/libs/*.class))
JAVA_CLASSES := $(wildcard $(JAVA_CLASSDIR)pocky/libs/*.class)
JAVA_HEADERS := $(subst /,_, $(subst $(JAVA_CLASSDIR), '', $(JAVA_CLASSES:.class=.h)))
JNI_CLASSES := $(subst $(JAVA_CLASSDIR), '', $(JAVA_CLASSES:.class=))
# Build
all : pocky
pineapple:
	make -C ../pineapple all
headergen : #generate jni headers
	@ \
	echo '\nCreating JNI Headers'; \
	echo $(JAVA_HEADERS); \
	cd bin/classes/; \
	javah -jni -d ../../jni/include $(subst /,., $(JNI_CLASSES)) 
pocky : pineapple headergen #compile
	@ \
	echo '\nCompiling'; \
	echo '$(CC) $(wildcard $(SRCDIR)/src/*.cpp)\n'; \
	$(CC) -j5
clean:
	@ \
	rm -rf obj/* libs/*;