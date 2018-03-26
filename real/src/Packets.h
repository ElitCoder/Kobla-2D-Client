#ifndef PACKETS_H
#define PACKETS_H

#include "Game.h"
#include "Login.h"

void PacketLogin(char *acc, char *pass, int w);

void PacketCharacterList();
void PacketCharacterChange(int w, char *c, char *pass, int i, int job);

void PacketMainSpawn(int id, Character *p);

void PacketGameMove(int DIR, int X, int Y);
void PacketGameStopMove(int x, int y);

void PacketGameChangeMap(int x, int y);

void PacketGameAttack(int id, int type, int param, int hits);

void PacketGameUpdatePosition(int x, int y);

void PacketIsOnline(int id);
void PacketQuit();

void PacketSendChat(char *msg);

void PacketGameNewBullet(Bullet *b, int PARAM);
void PacketGameEquipWeapon();

void PacketGameAddStat(int TYPE);

void PacketInviteToParty(char *name);
void PacketKickFromParty(char *name);
void PacketLeaveParty();

void PacketAcceptParty(int status, char *name);

void PacketNPCMenu(int npcId, int button);
void PacketHitDialogButton(int npcId, int button);

void PacketInventoryMoveItem(int oldSlot, int newSlot);
void PacketInventoryUseItem(int slot);
void PacketInventoryRemoveItem(int slot, int qty, int itemid);

void PacketPickUpDrop(int id);

void PacketUseSkill(int id);

void PacketAddTaskbarItem(int id, int type, int slot);
void PacketRemoveTaskbarItem(int id);
void PacketUseTaskbarItem(int puh);

void PacketDropItem(int slot, int qty, int itemid);

void PacketUnEquip(int slot);

void PacketSendPing();

void PacketShopAdd(int place, int qty, int npcId);
void PacketShopRemove(int place);
void PacketShopBuy(std::vector<int> itemid, std::vector<int> itemq, int npcid);
void PacketShopSell(int itemSell, int qty, int itemid);

void PacketErrorMessage(const char *msg);

void PacketCharacterMenuButton(char *text, int charId);

void PacketAcceptTradeInvite(int status, int name);

void PacketAcceptFriendInvite(int status, int name);

void PacketAddTradeItem(int slotadd, int qty, int itemid);
void PacketDoneTrade();
void PacketCancelTrade();

void PacketNPCTeleport(int npcId, int i);

void PacketChangeFriend(int status, const char *name);
void PacketFriendMenu(const char *name, const char *com);

void PacketSendPM(const char *name, const char *msg);

#endif