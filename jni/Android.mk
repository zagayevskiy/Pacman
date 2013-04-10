LOCAL_PATH := $(call my-dir)
 
include $(CLEAR_VARS)

LOCAL_LDLIBS    := -llog -lGLESv2 -landroid -ldl -lOpenSLES
LOCAL_CFLAGS    := -Werror -DANDROID_NDK -DDISABLE_IMPORTGL
LOCAL_MODULE    := pacman
LOCAL_SRC_FILES := \
	pacmanlib.cpp \
	graphics/IRenderable.cpp \
	graphics/ui/controls/Control.cpp \
	graphics/ui/controls/RectButton.cpp \
	graphics/ui/controls/Label.cpp \
	graphics/ui/controls/CheckBox.cpp \
	graphics/ui/menu/Menu.cpp \
	graphics/ui/menu/MainMenu.cpp \
	graphics/ui/menu/GameMenu.cpp \
	graphics/ui/menu/SwipeGameMenu.cpp \
	graphics/ui/menu/GameOverMenu.cpp \
	graphics/ui/menu/PauseMenu.cpp \
	graphics/ui/menu/WinMenu.cpp \
	graphics/animation/Animation.cpp \
	graphics/animation/Plume.cpp \
	managers/Art.cpp \
	managers/Audio.cpp \
	managers/Statistics.cpp \
	managers/Store.cpp \
	managers/ShadersManager.cpp \
	logic/Game.cpp \
	logic/Engine.cpp \
	logic/StateMachine.cpp \
	logic/actors/Actor.cpp \
	logic/actors/Pacman.cpp \
	logic/actors/monsters/Monster.cpp \
	logic/actors/monsters/StupidMonster.cpp \
	logic/actors/monsters/CleverMonster.cpp \
	logic/actors/bonuses/Bonus.cpp \
	logic/actors/bonuses/LifeBonus.cpp
 
include $(BUILD_SHARED_LIBRARY)