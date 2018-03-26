#include "Log.h"
#include "Base.h"
#include "PacketCreator.h"

using namespace std;

static void printStart() {
	Log(NONE) << "Kobla-2D-Client-Rebased [alpha] [" << __DATE__ << " @ " << __TIME__ << "]\n";
}

// TODO: Load up game from here
static void load() {
}

static void process() {
	Log(DEBUG) << "Getting config options for network\n";
	
	const auto& hostname = Base::settings().get<string>("host");
	const auto port = Base::settings().get<unsigned short>("port");
	
	Log(DEBUG) << "Starting network and connecting\n";
	Base::network().start(hostname, port);
	
	load();
	
	while (true) {
		while (true) {
			auto* packet = Base::network().waitForPacketFast();
			
			if (packet == nullptr)
				break;
				
			// Handle packet
			Base::game().process(*packet);
			
			Base::network().completePacket();
		}
		
		// Logic and render
		Base::game().logic();
		
		// Test packet
		//Base::network().send(PacketCreator::ping());
	}
}

int main() {
	printStart();
	
	Log(DEBUG) << "Parsing config\n";
	Base::settings().parse("config");
	
	process();
	
	return 0;
}