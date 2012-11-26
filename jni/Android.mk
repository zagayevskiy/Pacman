LOCAL_PATH := $(call my-dir)
 
include $(CLEAR_VARS)

LOCAL_LDLIBS    := -llog -lGLESv2 -landroid -ldl
LOCAL_CFLAGS    := -Werror -DANDROID_NDK -DDISABLE_IMPORTGL
LOCAL_MODULE    := pacman
LOCAL_SRC_FILES := \
	pacmanlib.cpp \
	graphics/IRenderable.cpp \
	graphics/ui/Control.cpp \
	graphics/ui/RectButton.cpp \
	graphics/ui/menu/Menu.cpp \
	graphics/ui/menu/MainMenu.cpp \
	graphics/ui/menu/GameMenu.cpp \
	graphics/ui/menu/SwipeGameMenu.cpp \
	graphics/ui/menu/GameOverMenu.cpp \
	graphics/ui/menu/PauseMenu.cpp \
	graphics/ui/menu/WinMenu.cpp \
	managers/Art.cpp \
	managers/ShadersManager.cpp \
	logic/Game.cpp \
	logic/Engine.cpp \
	logic/StateMachine.cpp \
	logic/actors/Actor.cpp \
	logic/actors/Pacman.cpp \
	logic/actors/Monster.cpp \
	logic/actors/StupidMonster.cpp
 
include $(BUILD_SHARED_LIBRARY)