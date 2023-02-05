cpp_files := $(wildcard src/*cpp)
o_files := $(foreach cpp_file, \
      $(cpp_files), \
      $(patsubst %.cpp, %.o, $(cpp_file)))


romviewer : $(o_files)
	g++ -std=c++17 -g -F/usr/local/Qt-6.6.0/lib -framework QtCore -framework QtWidgets -framework QtWidgets -framework QtGui -o romviewer $^;
	install_name_tool -change @rpath/QtGui.framework/Versions/A/QtGui /usr/local/Qt-6.6.0/lib/QtGui.framework/Versions/A/QtGui romviewer;
	install_name_tool -change @rpath/QtCore.framework/Versions/A/QtCore /usr/local/Qt-6.6.0/lib/QtCore.framework/Versions/A/QtCore romviewer; 
	install_name_tool -change @rpath/QtWidgets.framework/Versions/A/QtWidgets /usr/local/Qt-6.6.0/lib/QtWidgets.framework/Versions/A/QtWidgets romviewer;

src/%.o : src/%.cpp
	g++ -F/usr/local/Qt-6.6.0/lib -framework QtCore -framework QtWidgets -framework QtGui -std=c++17 -g -c -I./include $< -o $@;

.PHONY: clean
clean:
	rm src/*.o;
	rm romviewer;
