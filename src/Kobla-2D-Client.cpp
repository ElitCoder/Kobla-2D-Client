#include "Log.h"
#include "Base.h"
#include "PacketCreator.h"

using namespace std;

static mutex g_main_sync;

static void printStart() {
	Log(NONE) << "Kobla-2D-Client-Rebased [alpha] [" << __DATE__ << " @ " << __TIME__ << "]\n";
}

// TODO: Load up game from here
static void load() {
}

static void render() {
	while (true) {
		g_main_sync.lock();
		
		// Render here
		Base::game().logic();
		
		// Sleep since no graphics
		this_thread::sleep_for(chrono::milliseconds(10));
		
		g_main_sync.unlock();
	}
}

static void process() {
	Log(DEBUG) << "Getting config options for network\n";
	
	const auto& hostname = Base::settings().get<string>("host");
	const auto port = Base::settings().get<unsigned short>("port");
	
	Log(DEBUG) << "Starting network and connecting\n";
	Base::network().start(hostname, port);
	
	load();
	
	thread render_thread(render);

	while (true) {
		auto& packet = Base::network().waitForPacket();
		
		g_main_sync.lock();
		Base::game().process(packet);
		g_main_sync.unlock();
		
		Base::network().completePacket();
	}
	
	render_thread.detach();
}

int main() {
	printStart();
	
	Log(DEBUG) << "Parsing config\n";
	Base::settings().parse("config");
	
	process();
	
	return 0;
}