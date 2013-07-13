##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=savide
ConfigurationName      :=Release
WorkspacePath          := "E:\Program Files\CodeLite\Test"
ProjectPath            := "E:\Program Files\CodeLite\Test\savide"
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=rakib
Date                   :=7/13/2013
CodeLitePath           :="E:\Program Files\CodeLite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="savide.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=windres
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)soil $(LibrarySwitch)opengl32 $(LibrarySwitch)glut32 $(LibrarySwitch)box2d $(LibrarySwitch)glu32 
ArLibs                 :=  "libsoil" "libopengl32" "libglut32" "libbox2d" "libglu32" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -O2 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)


##
## User defined environment variables
##
CodeLiteDir:=E:\Program Files\CodeLite2\CodeLite
WXWIN:=E:\wxWidgets-2.9.4
PATH:=$(WXWIN)\lib\gcc_dll;$(PATH)
WXCFG:=gcc_dll\mswu
UNIT_TEST_PP_SRC_DIR:=E:\UnitTest++-1.3
MSVC := C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin
Objects0=$(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/bodies$(ObjectSuffix) $(IntermediateDirectory)/Render$(ObjectSuffix) $(IntermediateDirectory)/agent$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Program Files/CodeLite/Test/savide/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/bodies$(ObjectSuffix): bodies.cpp $(IntermediateDirectory)/bodies$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Program Files/CodeLite/Test/savide/bodies.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bodies$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bodies$(DependSuffix): bodies.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bodies$(ObjectSuffix) -MF$(IntermediateDirectory)/bodies$(DependSuffix) -MM "bodies.cpp"

$(IntermediateDirectory)/bodies$(PreprocessSuffix): bodies.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bodies$(PreprocessSuffix) "bodies.cpp"

$(IntermediateDirectory)/Render$(ObjectSuffix): Render.cpp $(IntermediateDirectory)/Render$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Program Files/CodeLite/Test/savide/Render.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Render$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Render$(DependSuffix): Render.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Render$(ObjectSuffix) -MF$(IntermediateDirectory)/Render$(DependSuffix) -MM "Render.cpp"

$(IntermediateDirectory)/Render$(PreprocessSuffix): Render.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Render$(PreprocessSuffix) "Render.cpp"

$(IntermediateDirectory)/agent$(ObjectSuffix): agent.cpp $(IntermediateDirectory)/agent$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Program Files/CodeLite/Test/savide/agent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/agent$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/agent$(DependSuffix): agent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/agent$(ObjectSuffix) -MF$(IntermediateDirectory)/agent$(DependSuffix) -MM "agent.cpp"

$(IntermediateDirectory)/agent$(PreprocessSuffix): agent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/agent$(PreprocessSuffix) "agent.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/bodies$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/bodies$(DependSuffix)
	$(RM) $(IntermediateDirectory)/bodies$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Render$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Render$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Render$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/agent$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/agent$(DependSuffix)
	$(RM) $(IntermediateDirectory)/agent$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) $(OutputFile).exe
	$(RM) "../.build-release/savide"


