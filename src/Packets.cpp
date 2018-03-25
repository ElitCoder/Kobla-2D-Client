#include "Packet.h"
#include "Packets.h"
#include "Misc.h"

extern int lastPing;

void PacketLogin(char *acc, char *pass, int w)
{
	Packet pak = Packet();

	if(w == 1)
		pak.addHeader(0x2);
	else if(w == 2)
		pak.addHeader(0x3);

	pak.addString(acc);
	pak.addString(pass);

	pak.ready();
}

void PacketCharacterList()
{
	Packet pak = Packet();

	pak.addHeader(0x6);

	pak.ready();
}

void PacketMainSpawn(int id, Character *p)
{
	Packet pak = Packet();

	pak.addHeader(0x5);
	pak.addInt(id);
	pak.addInt(p->my->w);
	pak.addInt(p->my->h);
	pak.addInt(21);
	pak.addInt(11);

	pak.ready();
}

void PacketCharacterChange(int w, char *c, char *pass, int i, int job)
{
	Packet a = Packet();

	a.addHeader(0x7);
	a.addInt(w);
	a.addString(c);

	if(w == 2)
		a.addString(pass);

	else
	{
		a.addInt(i);
		a.addInt(job);
	}

	a.ready();
}

void PacketGameMove(int DIR, int X, int Y)
{
	Packet a = Packet();

	a.addHeader(0x8);
	a.addInt(DIR);
	a.addLongInt(X);
	a.addLongInt(Y);

	a.ready();
}

void PacketGameStopMove(int x, int y)
{
	Packet a = Packet();

	a.addHeader(0x9);
	a.addLongInt(x);
	a.addLongInt(y);

	a.ready();
}

void PacketGameChangeMap(int x, int y)
{
	Packet a = Packet();

	a.addHeader(0x10);
	a.addLongInt(x);
	a.addLongInt(y);
	
	a.ready();
}

void PacketGameUpdatePosition(int x, int y)
{
	Packet a = Packet();

	a.addHeader(0x14);
	a.addLongInt(x);
	a.addLongInt(y);

	a.ready();
}

void PacketIsOnline(int id)
{
	Packet a = Packet();

	a.addHeader(0x55);
	a.addInt(id);

	a.ready();
}

void PacketQuit()
{
	Packet a = Packet();

	a.addHeader(0x56);
	
	a.ready();
}

void PacketSendChat(char *msg)
{
	Packet a = Packet();

	a.addHeader(0x15);
	a.addString(msg);

	a.ready();
}

void PacketGameNewBullet(Bullet *b, int PARAM)
{
	Packet a = Packet();

	a.addHeader(0x16);
	a.addInt(PARAM);
	a.addInt(b->cWay);
	a.addLongInt(getTime(false));
	a.addLongInt((int)b->cX);
	a.addLongInt((int)b->cY);

	a.ready();
}

void PacketGameEquipWeapon()
{
	Packet a = Packet();

	a.addHeader(0x17);
	
	a.ready();
}

void PacketGameAttack(int id, int type, int param, int hits)
{
	Packet a = Packet();

	a.addHeader(0x18);
	a.addLongInt(id);
	a.addInt(type);
	a.addInt(param);
	a.addLongInt(hits);

	a.ready();
}

void PacketGameAddStat(int TYPE)
{
	Packet a = Packet();

	a.addHeader(0x22);
	a.addInt(TYPE);

	a.ready();
}

void PacketInviteToParty(char *name)
{
	Packet a = Packet();

	a.addHeader(0x24);
	a.addString(name);

	a.ready();
}

void PacketLeaveParty()
{
	Packet a = Packet();

	a.addHeader(0x25);
	
	a.ready();
}

void PacketKickFromParty(char *name)
{
	Packet a = Packet();

	a.addHeader(0x26);
	a.addString(name);

	a.ready();
}

void PacketAcceptParty(int status, char *name)
{
	Packet a = Packet();

	a.addHeader(0x27);
	a.addInt(status);
	a.addString(name);

	a.ready();
}

void PacketAcceptTradeInvite(int status, int name)
{
	Packet a = Packet();

	a.addHeader(0x72);
	a.addInt(status);
	a.addLongInt(name);

	a.ready();
}

void PacketAcceptFriendInvite(int status, int name)
{
	Packet a = Packet();

	a.addHeader(0x0C);
	a.addInt(status);
	a.addLongInt(name);

	a.ready();
}

void PacketChangeFriend(int status, const char *name)
{
	Packet a = Packet();

	a.addHeader(0x0E);
	a.addInt(status);
	a.addString((char*)name);

	a.ready();
}

void PacketFriendMenu(const char *name, const char *com)
{
	Packet a = Packet();

	a.addHeader(0x0F);
	a.addString((char*)name);
	a.addString((char*)com);

	a.ready();
}

void PacketNPCMenu(int npcId, int button)
{
	Packet a = Packet();

	a.addHeader(0x30);
	a.addInt(button);
	a.addLongInt(npcId);

	a.ready();
}

void PacketHitDialogButton(int npcId, int button)
{
	Packet a = Packet();

	a.addHeader(0x33);
	a.addInt(button);
	a.addLongInt(npcId);

	a.ready();
}

void PacketInventoryMoveItem(int oldSlot, int newSlot)
{
	Packet a = Packet();

	a.addHeader(0x35);
	a.addInt(oldSlot);
	a.addInt(newSlot);

	a.ready();
}

void PacketInventoryUseItem(int slot)
{
	Packet a = Packet();

	a.addHeader(0x37);
	a.addInt(slot);

	a.ready();
}

void PacketInventoryRemoveItem(int slot, int qty, int itemid)
{
	Packet a = Packet();

	a.addHeader(0x38);
	a.addInt(slot);
	a.addLongInt(qty);
	a.addLongInt(itemid);

	a.ready();
}

void PacketPickUpDrop(int id)
{
	Packet a = Packet();

	a.addHeader(0x39);
	a.addLongInt(id);

	a.ready();
}

void PacketUseSkill(int id)
{
	Packet a = Packet();

	a.addHeader(0x45);
	a.addLongInt(id);

	a.ready();
}

void PacketAddTaskbarItem(int id, int type, int slot)
{
	Packet a = Packet();

	a.addHeader(0x49);
	a.addLongInt(id);
	a.addInt(type);
	a.addInt(slot);

	a.ready();
}

void PacketRemoveTaskbarItem(int id)
{
	Packet a = Packet();

	a.addHeader(0x48);
	a.addLongInt(id);

	a.ready();
}

void PacketUseTaskbarItem(int puh)
{
	Packet a = Packet();

	a.addHeader(0x50);
	a.addLongInt(puh);

	a.ready();
}

void PacketDropItem(int slot, int qty, int itemid)
{
	Packet a = Packet();

	a.addHeader(0x53);
	a.addInt(slot);
	a.addLongInt(qty);
	a.addLongInt(itemid);

	a.ready();
}

void PacketUnEquip(int slot)
{
	Packet a = Packet();

	a.addHeader(0x58);
	a.addInt(slot);

	a.ready();
}

void PacketSendPing()
{
	Packet a = Packet();

	a.addHeader(0x59);
	a.addLongInt(lastPing);

	a.ready();
}

void PacketShopAdd(int place, int qty, int npcId)
{
	Packet a = Packet();

	a.addHeader(0x65);
	a.addLongInt(npcId);
	a.addLongInt(place);
	a.addLongInt(qty);

	a.ready();
}

void PacketShopBuy(std::vector<int> itemid, std::vector<int> itemq, int npcid)
{
	Packet a = Packet();

	a.addHeader(0x66);
	a.addLongInt(npcid);
	a.addLongInt(itemid.size());

	for(unsigned int i = 0; i < itemid.size(); i++)
	{
		a.addLongInt(itemid.at(i));
		a.addLongInt(itemq.at(i));
	}

	a.ready();
}

void PacketShopSell(int itemSell, int qty, int itemid)
{
	Packet a = Packet();

	a.addHeader(0x67);
	a.addInt(itemSell);
	a.addLongInt(qty);
	a.addLongInt(itemid);

	a.ready();
}

void PacketShopRemove(int place)
{
	Packet a = Packet();

	a.addHeader(0x69);
	a.addLongInt(place);

	a.ready();
}

void PacketErrorMessage(const char *msg)
{
	Packet a = Packet();

	a.addHeader(0x68);
	a.addLongString((char*)msg);

	a.ready();
}

void PacketCharacterMenuButton(char *text, int charId)
{
	Packet a = Packet();

	a.addHeader(0x70);
	a.addLongInt(charId);
	a.addString(text);

	a.ready();
}

void PacketAddTradeItem(int slotadd, int qty, int itemid)
{
	Packet a = Packet();

	a.addHeader(0x75);
	a.addInt(slotadd);
	a.addLongInt(qty);
	a.addLongInt(itemid);

	a.ready();
}

void PacketDoneTrade()
{
	Packet a = Packet();

	a.addHeader(0x76);
	a.addInt(1);

	a.ready();
}

void PacketCancelTrade()
{
	Packet a = Packet();

	a.addHeader(0x76);
	a.addInt(2);

	a.ready();
}

void PacketNPCTeleport(int npcId, int i)
{
	Packet a = Packet();

	a.addHeader(0x78);
	a.addLongInt(npcId);
	a.addLongInt(i);

	a.ready();
}

void PacketSendPM(const char *name, const char *msg)
{
	Packet a = Packet();

	a.addHeader(0x1A);
	a.addString((char*)name);
	a.addString((char*)msg);

	a.ready();
}