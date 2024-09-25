#include "modding.h"
#include "global.h"
#include "overlays/actors/ovl_Obj_Warpstone/z_obj_warpstone.h"

RECOMP_IMPORT("*", void recomp_set_owl_saves_delete(bool new_val));
RECOMP_IMPORT("*", void recomp_set_owls_quit_game(bool new_val));

void Owl_Save(PlayState* play) {
    play->state.unk_A3 = 1;
    gSaveContext.save.isOwlSave = true;
    Play_SaveCycleSceneFlags((GameState*) play);
    func_8014546C(&play->sramCtx);

    if (gSaveContext.fileNum != 0xFF) {
        Sram_SetFlashPagesOwlSave(&play->sramCtx, gFlashOwlSaveStartPages[gSaveContext.fileNum * 2],
                                  gFlashOwlSaveNumPages[gSaveContext.fileNum * 2]);
        Sram_StartWriteToFlashOwlSave(&play->sramCtx);
    }
    gSaveContext.save.isOwlSave = false;
}

RECOMP_CALLBACK("*", recomp_on_owl_update)
void owls_never_quit(ObjWarpstone* this, PlayState* play)
{
    if (this->isTalking && play->msgCtx.currentTextId == 0xC01 && play->msgCtx.msgBufPos == 269) {
        // @onq Skip the text about quitting the game.
        u32 i;
        for (i = 0; i < 18; ++i) {
            play->msgCtx.font.msgBuf.schar[569 + i] = play->msgCtx.font.msgBuf.schar[588 + i];
        }
    }
}

RECOMP_CALLBACK("*", recomp_on_owl_save)
void and_never_will(ObjWarpstone* this, PlayState* play)
{
    recomp_set_owls_quit_game(false);
    recomp_set_owl_saves_delete(false);
}

RECOMP_CALLBACK("*", recomp_after_owl_save)
void deal_with_it(ObjWarpstone* this, PlayState* play)
{
    Owl_Save(play);
}