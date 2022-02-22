// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_STOREINBOX_H_074FB99DD3FEDB823AAD2D2CD6F10119
#define FS_STOREINBOX_H_074FB99DD3FEDB823AAD2D2CD6F10119

#include "container.h"

class StoreInbox final : public Container
{
	public:
		explicit StoreInbox(uint16_t type);

		StoreInbox* getStoreInbox() override {
			return this;
		}
		const StoreInbox* getStoreInbox() const override {
			return this;
		}

		//cylinder implementations
		ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count,
			uint32_t flags, Creature* actor = nullptr) const override;

		void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) override;
		void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t link = LINK_OWNER) override;

		bool canRemove() const override {
			return false;
		}
};

#endif
