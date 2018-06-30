all: main

CPP_FLAGS = -g -std=c++11 -O3 -Wall -Wno-reorder

CPP_LIBS = -ljson_spirit -lpng -pthread

define recipe
bin/$(1).o: src/$(2)$(1).cpp src/$(2)$(1).h
	g++ -c $(CPP_FLAGS) src/$(2)$(1).cpp -o bin/$(1).o
endef

bin/main.o: src/main.cpp
	g++ -c $(CPP_FLAGS) src/main.cpp -o bin/main.o

CAMS = OrthogonalCamera PerspectiveCamera
$(foreach i,$(CAMS),$(eval $(call recipe,$(i),Camera/)))

HIT = Triangle Sphere
$(foreach i,$(HIT),$(eval $(call recipe,$(i),Hitable/)))

IMG = Image
$(foreach i,$(IMG),$(eval $(call recipe,$(i),Image/)))

LIGHTS = DirectionalLight PointLight SpotLight SphereLight
$(foreach i,$(LIGHTS),$(eval $(call recipe,$(i),Light/)))

MAT = Texture
$(foreach i,$(MAT),$(eval $(call recipe,$(i),Material/)))

MATH = Vec2 Vec3 Vec4 Matrix4 Perlin Util
$(foreach i,$(MATH),$(eval $(call recipe,$(i),Math/)))

MESH = Mesh
$(foreach i,$(MESH),$(eval $(call recipe,$(i),Mesh/)))

PARSER = Parser SceneParser HitableParser ParserUtil
$(foreach i,$(PARSER),$(eval $(call recipe,$(i),Parser/)))

REF = BlinnPhong CookTorrance
$(foreach i,$(REF),$(eval $(call recipe,$(i),Reflection/)))

RENDS = Renderer RayTracer PathTracer
$(foreach i,$(RENDS),$(eval $(call recipe,$(i),Renderer/)))

TREE = Box CubeTree
$(foreach i,$(TREE),$(eval $(call recipe,$(i),Tree/)))

FOLDERS = Camera Image Hitable Light Material Math Mesh Parser Reflection Renderer Tree
SRC = $(CAMS) $(HIT) $(IMG) $(LIGHTS) $(MAT) $(MATH) $(MESH) $(PARSER) $(REF) $(RENDS) $(TREE)

RECIPES = bin/main.o $(foreach i, $(SRC), bin/$(i).o)

main: $(RECIPES)
	g++ -o main $(RECIPES) $(CPP_LIBS)
	rm -f src/*h.gch
	$(foreach i,$(FOLDERS), rm -f src/$(i)/*.h.gch)

clean:
	rm -f bin/*.o
	rm -f src/*h.gch
	$(foreach i,$(FOLDERS), rm -f src/$(i)/*.h.gch)