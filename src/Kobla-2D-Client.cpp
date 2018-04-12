#include "Log.h"
#include "Base.h"
#include "PacketCreator.h"

using namespace std;

mutex g_main_sync;

static void printStart() {
	Log(NONE) << "Kobla-2D-Client-Rebased [alpha] [" << __DATE__ << " @ " << __TIME__ << "]\n";
}

// TODO: Load up game from here
static void load() {
	Base::engine().start();
	Base::engine().load();
}

static void packetThread() {
	Log(DEBUG) << "Starting packet thread\n";
	
	while (true) {	
		auto& packet = Base::network().waitForPacket();
		
		g_main_sync.lock();
		Base::game().process(packet);
		g_main_sync.unlock();
		
		Base::network().completePacket();
	}
}

static void process() {
	Log(DEBUG) << "Getting config options for network\n";
	
	const auto& hostname = Base::settings().get<string>("host");
	const auto port = Base::settings().get<unsigned short>("port");
	
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
	
	Log(DEBUG) << "Parsing config\n";
	Base::settings().parse("config");
	
	process();
	
	return 0;
}