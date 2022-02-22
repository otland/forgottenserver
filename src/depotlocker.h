// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_DEPOTLOCKER_H_53AD8E0606A34070B87F792611F4F3F8
#define FS_DEPOTLOCKER_H_53AD8E0606A34070B87F792611F4F3F8

#include "container.h"
#include "inbox.h"

using DepotLocker_ptr = std::shared_ptr<DepotLocker>;

class DepotLocker final : public Container
{
	public:
		explicit DepotLocker(uint16_t type);

		DepotLocker* getDepotLocker() override {
			return this;
		}
		const DepotLocker* getDepotLocker() const override {
			return this;
		}

		void removeInbox(Inbox* inbox);

		//serialization
		Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) override;

		uint16_t getDepotId() const {
			return depotId;
		}
		void setDepotId(uint16_t depotId) {
			this->depotId = depotId;
		}

		//cylinder implementations
		ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count,
				uint32_t flags, Creature* actor = nullptr) const override;

		void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) override;
		void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t link = LINK_OWNER) override;

		bool canRemove() const override {
			return false;
		}

	private:
		uint16_t depotId;
};

#endif

