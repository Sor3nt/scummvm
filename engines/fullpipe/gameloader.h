/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef FULLPIPE_GAMELOADER_H
#define FULLPIPE_GAMELOADER_H

#include "common/ptr.h"
#include "engines/savestate.h"

#include "fullpipe/objects.h"
#include "fullpipe/inventory.h"
#include "fullpipe/messages.h"

namespace Fullpipe {

#define FULLPIPE_SAVEGAME_VERSION 1

class SceneTag;
class MctlCompound;
class InputController;
class InteractionController;
class MotionController;
class MovGraph;

class Sc2 : public CObject {
 public:
	int16 _sceneId;
	int16 _field_2;
	Scene *_scene;
	/** owned */
	MotionController *_motionController;
	Common::Array<int32> _data1; // FIXME, could be a struct
	PicAniInfoList _defPicAniInfos;
	PicAniInfoList _picAniInfos;
	bool _isLoaded;
	Common::Array<EntranceInfo> _entranceData;

 public:
	Sc2();
	virtual ~Sc2();
	virtual bool load(MfcArchive &file);
};

typedef Common::Array<Sc2> Sc2Array;

struct PreloadItem {
	int preloadId1;
	int preloadId2;
	int sceneId;
	int param;
};

bool preloadCallback(PreloadItem &pre, int flag);

class PreloadItems : public Common::Array<PreloadItem>, public CObject {
 public:
	virtual bool load(MfcArchive &file);
};

struct FullpipeSavegameHeader {
	char id[6];
	uint8 version;
	Common::String saveName;
	uint32 date;
	uint16 time;
	uint32 playtime;
	Common::SharedPtr<Graphics::Surface> thumbnail;
};

struct SaveHeader {
	int32 version;
	char magic[32];
	int32 updateCounter;
	int32 unkField;
	int32 encSize;
};

class GameLoader : public CObject {
 public:
	GameLoader();
	virtual ~GameLoader();

	virtual bool load(MfcArchive &file);
	bool loadScene(int sceneId);
	bool gotoScene(int sceneId, int entranceId);
	bool preloadScene(int sceneId, int entranceId);
	bool unloadScene(int sceneId);

	void addPreloadItem(const PreloadItem &item);

	void updateSystems(int counterdiff);

	int getSceneTagBySceneId(int sceneId, SceneTag **st);
	void applyPicAniInfos(Scene *sc, const PicAniInfoList &picAniInfo);
	void saveScenePicAniInfos(int sceneId);
	PicAniInfoList savePicAniInfos(Scene *sc, int flag1, int flag2);

	bool readSavegame(const char *fname);
	bool writeSavegame(Scene *sc, const char *fname);

	void addVar(GameVar *var, GameVar *subvar);

	void restoreDefPicAniInfos();

	Common::ScopedPtr<GameProject> _gameProject;
	InteractionController *_interactionController;
	InputController *_inputController;
	Inventory2 _inventory;
	Sc2Array _sc2array;
	void *_sceneSwitcher;
	bool (*_preloadCallback)(PreloadItem &pre, int flag);
	void (*_savegameCallback)(MfcArchive *archive, bool mode);
	int16 _field_F8;
	int16 _field_FA;
	PreloadItems _preloadItems;
	GameVar *_gameVar;
	Common::String _gameName;
	ExCommand _exCommand;
	int _updateCounter;
	int _preloadSceneId;
	int _preloadEntranceId;
};

const char *getSavegameFile(int saveGameIdx);
bool readSavegameHeader(Common::InSaveFile *in, FullpipeSavegameHeader &header);
void parseSavegameHeader(Fullpipe::FullpipeSavegameHeader &header, SaveStateDescriptor &desc);

Inventory2 *getGameLoaderInventory();
InteractionController *getGameLoaderInteractionController();
MctlCompound *getSc2MctlCompoundBySceneId(int16 sceneId);
MovGraph *getSc2MovGraphBySceneId(int16 sceneId);
MctlCompound *getCurrSceneSc2MotionController();

} // End of namespace Fullpipe

#endif /* FULLPIPE_GAMELOADER_H */
