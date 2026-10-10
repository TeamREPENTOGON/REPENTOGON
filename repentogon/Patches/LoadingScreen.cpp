#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "Log.h"
#include "LoadingScreen.h"

#include <chrono>
#include <mutex>
#include <Windows.h>
#include <gl/gl.h>

/* When Lua mods are loading, the game doesn't render anything, making the process look frozen.
 * We updated the title bar to show the current loading mod, but this doesn't help for Steam Deck.
 * Instead, we can replicate what the (patched out) Workshop update process does,
 * and we can reuse some of its plumbing to make this work.                                        */
namespace LoadingScreen {
	using Clock = std::chrono::steady_clock;

	static constexpr auto frameTime = std::chrono::microseconds(1000000 / 60);

	static std::atomic<bool> active = false;
	static std::mutex statusMutex;
	static std::string status;
	static bool statusChanged = false;

	bool IsActive() {
		return active;
	}

	void SetStatus(const char* text) {
		std::lock_guard lock(statusMutex);
		status = text;
		statusChanged = true;
	}

	static void ApplyStatus(ModManager* modManager) {
		std::lock_guard lock(statusMutex);
		if (statusChanged) {
			modManager->_updateStatusText = status;
			statusChanged = false;
		}
	}

	static void StartAnimation(ModManager* modManager) {
		ANM2* sprite = &modManager->_updateSprite;
		if (!sprite->_loaded) {
			std_string path("gfx/ui/loading.anm2");
			sprite->Load(path, true);
		}

		static const char* const anims[] = { "1", "2", "3", "4" };
		sprite->Play(anims[Isaac::genrand_int32() & 3], true);
	}

	static bool RunWithLoadingScreen(const std::function<void()>& work) {
		HDC dc = wglGetCurrentDC();
		HGLRC mainContext = wglGetCurrentContext();
		if (!dc || !mainContext) {
			ZHL::Log("[REPENTOGON] Loading screen: no current GL context, loading without it\n");
			return false;
		}

		HGLRC workerContext = wglCreateContext(dc);
		if (!workerContext || !wglShareLists(mainContext, workerContext)) {
			ZHL::Log("[REPENTOGON] Loading screen: couldn't create a shared GL context (error %lu), loading without it\n", GetLastError());
			if (workerContext) {
				wglDeleteContext(workerContext);
			}
			return false;
		}

		ModManager* modManager = g_Manager->GetModManager();
		StartAnimation(modManager);
		modManager->_updatingMods = true;
		active = true;

		std::atomic<bool> done = false;
		std::thread worker([&]() {
			wglMakeCurrent(dc, workerContext);
			work();
			glFinish();
			wglMakeCurrent(NULL, NULL);
			done = true;
		});

		unsigned int frame = 0;
		while (!done) {
			const Clock::time_point frameStart = Clock::now();

			KAGE_EngineUpdate(true);
			if ((++frame & 1) == 0) {
				modManager->_updateSprite.Update();
			}
			ApplyStatus(modManager);
			g_Manager->Render();

			std::this_thread::sleep_until(frameStart + frameTime);
		}

		worker.join();
		wglDeleteContext(workerContext);

		modManager->_updatingMods = false;
		active = false;

		g_KAGE_Graphics_Manager._boundImage = nullptr;
		g_KAGE_Graphics_Manager._shader = nullptr;
		return true;
	}

	static bool startupDone = false;
}

HOOK_METHOD_PRIORITY(ModManager, LoadConfigs, INT_MIN, () -> void) {
	luaL_dostring(g_LuaEngine->_state, "jit.off()"); // thread safety is my passion
	if (LoadingScreen::startupDone) {
		super();
		return;
	}
	LoadingScreen::startupDone = true;

	if (!LoadingScreen::RunWithLoadingScreen([this]() { super(); })) {
		super();
	}
	luaL_dostring(g_LuaEngine->_state, "jit.on()");
}