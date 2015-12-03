#ifndef FS_REWARD_H
#define FS_REWARD_H

#include "container.h"

class Reward :
	public Container
{
public:
	explicit Reward();
	
	//cylinder implementations
	ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count,
		uint32_t flags, Creature* actor = nullptr) const final;

	void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) final;
	void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t link = LINK_OWNER) final;

	//overrides
	bool canRemove() const final {
		return true;
	}

	Cylinder* getParent() const final;
	Cylinder* getRealParent() const final {
		return parent;
	}
};

#endif

