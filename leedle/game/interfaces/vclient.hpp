#pragma once

#include "interface.hpp"
#include "../types/clientclass.hpp"

namespace game {

enum RenderViewInfo {
	RENDERVIEW_UNSPECIFIED = 0,
	RENDERVIEW_DRAWVIEWMODEL = (1 << 0),
	RENDERVIEW_DRAWHUD = (1 << 1),
	RENDERVIEW_SUPPRESSMONITORRENDERING = (1 << 2),
};

enum ClientFrameStage
{
	FRAME_UNDEFINED=-1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
		FRAME_NET_UPDATE_POSTDATAUPDATE_START,
		FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,		
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

struct Rect {
	int x, y;
	int width, height;
};

struct VRect {
	int	x, y, width, height;
	VRect* pnext;
};

class VClient017 : public game::Interface<VClient017> {
public:
    virtual void init(void) = 0;
	virtual void post_init(void) = 0;
	virtual void shutdown(void) = 0;
	virtual void unk1(void) = 0;
	virtual void reply_post_init(void) = 0;
	virtual void level_init_pre_entity() = 0;
	virtual void level_init_post_entity() = 0;
	virtual void level_shutdown() = 0;
	virtual ClientClass* get_all_classes() = 0;

    static auto capture_interface() {
        return create_interface(memory::client, "VClient017");
    }
};

DEFINE_INTERFACE(VClient017, CLIENT);

}