MKDIR	:= mkdir -p
AR		:= ar
RANLIB := ranlib

CXXFLAGS := -std=c++17
LDFLAGS := -lasound


objectList = $(patsubst %.cpp,%.o,$(shell find . -name "*.cpp"))



APP := check
OBJECTS := $(objectList)



.PHONY: all
all:	app


.PHONY: app
app:	$(APP)

$(APP): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(APP_LIBS) $^ -o $@ 



$(LIB): $(LIB_OBJECTS)
	$(AR) crv $@ $^
	$(RANLIB) $@

.PHONY: clean
clean:
	rm -f $(LIB_OBJECTS) $(OBJECTS)

.PHONY: distclean
distclean:	clean
	rm -f $(LIB) $(APP)

.PHONY: list	
list:
	$(info App objects is $(OBJECTS))

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@