CPPFLAGS=-g -std=c++11
LDFLAGS=-g
SOURCES=Camera.cpp Color.cpp Helpers.cpp Matrix4.cpp Mesh.cpp Rotation.cpp Scaling.cpp Scene.cpp Translation.cpp Triangle.cpp Vec3.cpp Vec4.cpp tinyxml2.cpp Line.cpp Face.cpp
MAIN=Main.cpp
HEADERS=$(subst .cpp,.h,$(SOURCES))
OBJS=$(addprefix ./objs/, $(subst .cpp,.o,$(SOURCES)))
OBJ_DIR=objs
TESTS_DIR=tests
MAIN_TEST=$(TESTS_DIR)/MainTest.cpp

rasterizer: $(OBJS) $(subst .cpp,.o,$(MAIN))
	g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(OBJ_DIR)
	g++ $(CPPFLAGS) -c $^ -o $@

$(TESTS_DIR)/test: $(OBJS) $(subst .cpp,.o,$(MAIN_TEST))
	g++ $(LDFLAGS) -o $@ $^

$(TESTS_DIR)/%.o: $(TESTS_DIR)/%.cpp
	mkdir -p $(TESTS_DIR)
	g++ $(CPPFLAGS) -c $^ -o $@

clean:
	rm -r -f .\objs *.o
