
ANTZ_OBJS = \
	main.o \
	ctrl/npcmd.o \
	ctrl/npengine.o \
	ctrl/nptest.o \
	data/npmap.o \
	data/npmapfile.o \
	data/npnode.o \
	io/db/npauxdb.o \
	io/db/npmysql.o \
	io/file/npcsv.o \
	io/file/npgdal.o \
	io/file/npjson.o \
	io/file/npkml.o \
	io/file/npobj.o \
	io/file/npxml.o \
	io/gl/npgeometry.o \
	io/gl/npgldraw.o \
	io/gl/nptags.o \
	io/gl/nptexmap.o \
	io/npch.o \
	io/npdb.o \
	io/npfile.o \
	io/npgl.o \
	io/npglut.o \
	io/npkey.o \
	io/npmouse.o \
	lib/soil/src/SOIL.o \
	lib/soil/src/image_DXT.o \
	lib/soil/src/image_helper.o \
	lib/soil/src/stb_image_aug.o \
	npctrl.o \
	npdata.o \
	npio.o \
	os/linux/nplinux.o \
	os/msw/npmsw.o \
	os/npos.o \
	os/osx/nposx.o

antz: $(ANTZ_OBJS)
	gcc -g -lm -lGLU -lglut -o Release/antz $(ANTZ_OBJS)

%.o : %.c
	gcc -g -Wall -c $^ -o $@

clean: 
	rm $(ANTZ_OBJS)
