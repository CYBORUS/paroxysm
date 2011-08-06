#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1445227201/ClusterVBO.o \
	${OBJECTDIR}/_ext/1445227201/Font.o \
	${OBJECTDIR}/_ext/1853469074/ActorNode.o \
	${OBJECTDIR}/_ext/1445227201/PropertyList.o \
	${OBJECTDIR}/_ext/1445227201/StopWatch.o \
	${OBJECTDIR}/_ext/1445227201/Thread.o \
	${OBJECTDIR}/_ext/1852495333/GLee.o \
	${OBJECTDIR}/_ext/1445227201/Sound.o \
	${OBJECTDIR}/_ext/1445227201/Music.o \
	${OBJECTDIR}/_ext/1445227201/VertexShader.o \
	${OBJECTDIR}/_ext/1445227201/VertexBufferObject.o \
	${OBJECTDIR}/_ext/1445227201/IndexVBO.o \
	${OBJECTDIR}/_ext/1445227201/Engine.o \
	${OBJECTDIR}/_ext/1445227201/c3m.o \
	${OBJECTDIR}/_ext/1853469074/MapEditorModule.o \
	${OBJECTDIR}/_ext/1445227201/Download.o \
	${OBJECTDIR}/_ext/1445227201/ManagedModule.o \
	${OBJECTDIR}/_ext/1445227201/Image.o \
	${OBJECTDIR}/_ext/1445227201/Module.o \
	${OBJECTDIR}/_ext/1853469074/RenderBin.o \
	${OBJECTDIR}/_ext/1445227201/Mutex.o \
	${OBJECTDIR}/_ext/1853469074/GeneralBin.o \
	${OBJECTDIR}/_ext/1853469074/ViewNode.o \
	${OBJECTDIR}/_ext/1853469074/SceneGraphNode.o \
	${OBJECTDIR}/_ext/1445227201/Model.o \
	${OBJECTDIR}/_ext/1445227201/Texture.o \
	${OBJECTDIR}/_ext/1445227201/FragmentShader.o \
	${OBJECTDIR}/_ext/1853469074/SimpleMatrixNode.o \
	${OBJECTDIR}/_ext/1445227201/Tools.o \
	${OBJECTDIR}/_ext/1445227201/NetworkStream.o \
	${OBJECTDIR}/_ext/1853469074/TestActorNode.o \
	${OBJECTDIR}/_ext/1445227201/Texture2D.o \
	${OBJECTDIR}/_ext/1445227201/CubeMap.o \
	${OBJECTDIR}/_ext/1853469074/TerrainGrid.o \
	${OBJECTDIR}/_ext/1445227201/LuaMachine.o \
	${OBJECTDIR}/_ext/1853469074/main.o \
	${OBJECTDIR}/_ext/1445227201/VertexArrayObject.o \
	${OBJECTDIR}/_ext/1445227201/Camera.o \
	${OBJECTDIR}/_ext/1445227201/Shader.o \
	${OBJECTDIR}/_ext/1445227201/Exception.o \
	${OBJECTDIR}/_ext/1445227201/Graphics.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lSDLmain -lSDL_image -lSDL_ttf -lSDL_net -lSDL_mixer -lSDL -lGL -llua5.1 -lcurl

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../../paroxysm-${CND_DISTDIR}-${CND_CONF}-${CND_PLATFORM}

../../../paroxysm-${CND_DISTDIR}-${CND_CONF}-${CND_PLATFORM}: ${OBJECTFILES}
	${MKDIR} -p ../../..
	${LINK.cc} -o ../../../paroxysm-${CND_DISTDIR}-${CND_CONF}-${CND_PLATFORM} ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1445227201/ClusterVBO.o: ../../CGE/ClusterVBO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/ClusterVBO.o ../../CGE/ClusterVBO.cpp

${OBJECTDIR}/_ext/1445227201/Font.o: ../../CGE/Font.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Font.o ../../CGE/Font.cpp

${OBJECTDIR}/_ext/1853469074/ActorNode.o: ../../game/ActorNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1853469074
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1853469074/ActorNode.o ../../game/ActorNode.cpp

${OBJECTDIR}/_ext/1445227201/PropertyList.o: ../../CGE/PropertyList.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/PropertyList.o ../../CGE/PropertyList.cpp

${OBJECTDIR}/_ext/1445227201/StopWatch.o: ../../CGE/StopWatch.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/StopWatch.o ../../CGE/StopWatch.cpp

${OBJECTDIR}/_ext/1445227201/Thread.o: ../../CGE/Thread.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Thread.o ../../CGE/Thread.cpp

${OBJECTDIR}/_ext/1852495333/GLee.o: ../../GLee/GLee.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1852495333
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1852495333/GLee.o ../../GLee/GLee.c

${OBJECTDIR}/_ext/1445227201/Sound.o: ../../CGE/Sound.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Sound.o ../../CGE/Sound.cpp

${OBJECTDIR}/_ext/1445227201/Music.o: ../../CGE/Music.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Music.o ../../CGE/Music.cpp

${OBJECTDIR}/_ext/1445227201/VertexShader.o: ../../CGE/VertexShader.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/VertexShader.o ../../CGE/VertexShader.cpp

${OBJECTDIR}/_ext/1445227201/VertexBufferObject.o: ../../CGE/VertexBufferObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/VertexBufferObject.o ../../CGE/VertexBufferObject.cpp

${OBJECTDIR}/_ext/1445227201/IndexVBO.o: ../../CGE/IndexVBO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/IndexVBO.o ../../CGE/IndexVBO.cpp

${OBJECTDIR}/_ext/1445227201/Engine.o: ../../CGE/Engine.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Engine.o ../../CGE/Engine.cpp

${OBJECTDIR}/_ext/1445227201/c3m.o: ../../CGE/c3m.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/c3m.o ../../CGE/c3m.c

${OBJECTDIR}/_ext/1853469074/MapEditorModule.o: ../../game/MapEditorModule.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1853469074
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1853469074/MapEditorModule.o ../../game/MapEditorModule.cpp

${OBJECTDIR}/_ext/1445227201/Download.o: ../../CGE/Download.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Download.o ../../CGE/Download.cpp

${OBJECTDIR}/_ext/1445227201/ManagedModule.o: ../../CGE/ManagedModule.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/ManagedModule.o ../../CGE/ManagedModule.cpp

${OBJECTDIR}/_ext/1445227201/Image.o: ../../CGE/Image.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Image.o ../../CGE/Image.cpp

${OBJECTDIR}/_ext/1445227201/Module.o: ../../CGE/Module.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Module.o ../../CGE/Module.cpp

${OBJECTDIR}/_ext/1853469074/RenderBin.o: ../../game/RenderBin.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1853469074
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1853469074/RenderBin.o ../../game/RenderBin.cpp

${OBJECTDIR}/_ext/1445227201/Mutex.o: ../../CGE/Mutex.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Mutex.o ../../CGE/Mutex.cpp

${OBJECTDIR}/_ext/1853469074/GeneralBin.o: ../../game/GeneralBin.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1853469074
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1853469074/GeneralBin.o ../../game/GeneralBin.cpp

${OBJECTDIR}/_ext/1853469074/ViewNode.o: ../../game/ViewNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1853469074
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1853469074/ViewNode.o ../../game/ViewNode.cpp

${OBJECTDIR}/_ext/1853469074/SceneGraphNode.o: ../../game/SceneGraphNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1853469074
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1853469074/SceneGraphNode.o ../../game/SceneGraphNode.cpp

${OBJECTDIR}/_ext/1445227201/Model.o: ../../CGE/Model.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Model.o ../../CGE/Model.cpp

${OBJECTDIR}/_ext/1445227201/Texture.o: ../../CGE/Texture.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Texture.o ../../CGE/Texture.cpp

${OBJECTDIR}/_ext/1445227201/FragmentShader.o: ../../CGE/FragmentShader.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/FragmentShader.o ../../CGE/FragmentShader.cpp

${OBJECTDIR}/_ext/1853469074/SimpleMatrixNode.o: ../../game/SimpleMatrixNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1853469074
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1853469074/SimpleMatrixNode.o ../../game/SimpleMatrixNode.cpp

${OBJECTDIR}/_ext/1445227201/Tools.o: ../../CGE/Tools.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Tools.o ../../CGE/Tools.cpp

${OBJECTDIR}/_ext/1445227201/NetworkStream.o: ../../CGE/NetworkStream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/NetworkStream.o ../../CGE/NetworkStream.cpp

${OBJECTDIR}/_ext/1853469074/TestActorNode.o: ../../game/TestActorNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1853469074
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1853469074/TestActorNode.o ../../game/TestActorNode.cpp

${OBJECTDIR}/_ext/1445227201/Texture2D.o: ../../CGE/Texture2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Texture2D.o ../../CGE/Texture2D.cpp

${OBJECTDIR}/_ext/1445227201/CubeMap.o: ../../CGE/CubeMap.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/CubeMap.o ../../CGE/CubeMap.cpp

${OBJECTDIR}/_ext/1853469074/TerrainGrid.o: ../../game/TerrainGrid.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1853469074
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1853469074/TerrainGrid.o ../../game/TerrainGrid.cpp

${OBJECTDIR}/_ext/1445227201/LuaMachine.o: ../../CGE/LuaMachine.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/LuaMachine.o ../../CGE/LuaMachine.cpp

${OBJECTDIR}/_ext/1853469074/main.o: ../../game/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1853469074
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1853469074/main.o ../../game/main.cpp

${OBJECTDIR}/_ext/1445227201/VertexArrayObject.o: ../../CGE/VertexArrayObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/VertexArrayObject.o ../../CGE/VertexArrayObject.cpp

${OBJECTDIR}/_ext/1445227201/Camera.o: ../../CGE/Camera.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Camera.o ../../CGE/Camera.cpp

${OBJECTDIR}/_ext/1445227201/Shader.o: ../../CGE/Shader.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Shader.o ../../CGE/Shader.cpp

${OBJECTDIR}/_ext/1445227201/Exception.o: ../../CGE/Exception.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Exception.o ../../CGE/Exception.cpp

${OBJECTDIR}/_ext/1445227201/Graphics.o: ../../CGE/Graphics.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445227201
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/include/SDL -I/usr/include/lua5.1 -I../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445227201/Graphics.o ../../CGE/Graphics.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../../paroxysm-${CND_DISTDIR}-${CND_CONF}-${CND_PLATFORM}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
