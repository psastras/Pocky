# C/C++ Build variables
CC  = ndk-build
SRCDIR := jni
OBJDIR := obj


# Java build variables
JAVA_CLASSDIR := bin/classes/
JAVA_CLASSES := $(wildcard $(JAVA_CLASSDIR)pineapple/libs/*.class)
JAVA_HEADERS := $(JAVA_CLASSES:.class=)
JNI_CLASSES := $(subst $(JAVA_CLASSDIR), '', $(JAVA_HEADERS))
TEST := $(subst /,., $(JNI_CLASSES))
# Build
all : pineapple
headergen: #generate jni headers
	@ \
	rm -rf obj/local; \
	echo 'Creating JNI Headers'; \
	cd bin/classes/; \
	javah -jni -d ../../jni/include $(subst /,., $(JNI_CLASSES)) 
pineapple: headergen #compile
	@ \
	echo '\nCompiling'; \
	echo '$(CC) $(wildcard $(SRCDIR)/src/*.cpp)\n'; \
	$(CC)
	