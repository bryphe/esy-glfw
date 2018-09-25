BUILDDIR=$(ROOTDIR)/_build

ifeq (, $(shell which x86_64-w64-mingw32-g++))
INCLUDE=$(BUILDDIR)/glfw/include
LIBRARY=$(BUILDDIR)/glfw/src
else
INCLUDE=$(ROOTDIR)/include
LIBRARY=$(ROOTDIR)/lib-mingw-w64
ADDITIONAL_OPTS=-cclib -lgdi32
endif

# Building glfw from source
$(INCLUDE)/GLFW/glfw3.h:
	echo Library: $(LIBRARY)
	echo Include: $(INCLUDE)
	mkdir -p $(BUILDDIR)
	git clone https://github.com/glfw/glfw $(BUILDDIR)/glfw

$(LIBRARY)/libglfw3.a: $(INCLUDE)/GLFW/glfw3.h
	cd $(BUILDDIR)/glfw; cmake .
	cd $(BUILDDIR)/glfw; make

build-glfw: $(LIBRARY)/libglfw3.a
	echo Library: $(LIBRARY)
	echo Include: $(INCLUDE)
	mkdir -p $(BUILDDIR)

install:
	@echo Installing from $(LIBRARY) to $(LIBDIR)
	@mkdir -p $(LIBDIR)
	@cp $(LIBRARY)/*.a $(LIBDIR)

noop:
	@echo Using prebuilt binaries on Windows.
