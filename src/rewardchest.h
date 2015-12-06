#ifndef FS_REWARDCHEST_H
#define FS_REWARDCHEST_H

#include "container.h"

class RewardChest final : public Container
{
public:
	explicit RewardChest(uint16_t _type);

	RewardChest* getRewardChest() final {
		return this;
	}
	const RewardChest* getRewardChest() const final {
		return this;
	}

	//cylinder implementations
	ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count,
		uint32_t flags, Creature* actor = nullptr) const final;

	void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) final;
	void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t link = LINK_OWNER) final;

	bool canRemove() const final {
		return false;
	}
};

#endif
