#include "Log.h"
#include "Base.h"
#include "PacketCreator.h"
#include "Engine.h"
#include "NetworkCommunication.h"
#include "Game.h"
#include "Packet.h"

using namespace std;

mutex g_main_sync;

static void printStart() {
	Log(NONE) << "Kobla-2D-Client-Rebased [alpha] [" << __DATE__ << " @ " << __TIME__ << "]\n";
}

// TODO: Load up game from here
static void load() {
	Base::engine().start();
	Base::engine().load();
	
	// Run warm
	auto run_warm = Base::settings().get<bool>("run_warm", 1);
	
	if (run_warm)
		Base::engine().runWarm();
}

static void packetThread() {
	Log(DEBUG) << "Starting packet thread\n";
	
	while (true) {
		// Wait until the Server sends something
		auto* packet = Base::network().waitForPacket();
		
		if (packet == nullptr)
			continue;

		// Lock the game, we have a Packet
		g_main_sync.lock();
		Base::game().process(*packet);
		Base::network().completePacket();
		g_main_sync.unlock();
	}
}

static void process() {
	const string STANDARD_HOST = "localhost";
	const unsigned short STANDARD_PORT = 11000;
	
	Log(DEBUG) << "Getting config options for network\n";
	
	auto hostname = Base::settings().get<string>("host", STANDARD_HOST);
	auto port = Base::settings().get<unsigned short>("port", STANDARD_PORT);
	
	Log(DEBUG) << "Starting network and connecting\n";
	Base::network().start(hostname, port);
	
	load();
	
	thread packet_thread(packetThread);
	
	// Send spawn packet here for now
	Base::network().send(PacketCreator::spawn());
	
	Log(DEBUG) << "Starting rendering\n";
	
	// Rendering clock for animations
	sf::Clock frame_clock;
	
	while (Base::engine().running()) {
		g_main_sync.lock();
		
		Base::game().logic(frame_clock);
		Base::engine().render();
		
		// Unlock after drawing everything instead
		//g_main_sync.unlock();
	}
	
	// If the game loop is ended, avoid SIGABRT on exit
	packet_thread.detach();
}

int main() {
	printStart();
	
	Base::settings().parse("config");
	
	Log::setDebug(Base::settings().get<bool>("enable_debug", true));
	
	process();
	
	return 0;
}