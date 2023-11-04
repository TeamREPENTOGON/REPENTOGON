#include "imgui.h"
#include "IconsFontAwesome6.h"

#include <chrono>
#include <string>
#include <deque>

extern bool imguiResized;
extern ImVec2 imguiSizeModifier;

using namespace std::chrono;

struct Notification {
	int _severity = 0;
	std::string _text = "";
	int _lifetime = 5000;
	long long _creationTime = 0;
	long long _deletionTime = 0;

	Notification(const char* text = "", int severity = 0, int lifetime = 5000)
	{
		_severity = severity;
		_text = std::string(text);
		_lifetime = lifetime;
		_creationTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		_deletionTime = _creationTime + _lifetime;
	}
};

struct NotificationHandler {
	bool enabled = true;
	int animationTime = 2000;
	std::deque<Notification*>* notifications = new std::deque<Notification*>();

	const char* const severityIcons[4] = { ICON_FA_CIRCLE_INFO, ICON_FA_CIRCLE_CHECK, ICON_FA_CIRCLE_EXCLAMATION, ICON_FA_CIRCLE_XMARK };
	const ImVec4 severityColors[4] = { ImVec4(0.25f, 0.75f, 1.0f, 1.0f), ImVec4(0.25f, 1.0f, 0.25f, 1.0f), ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.15f, 0.15f, 1.0f) };

	const int windowFlags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;

	NotificationHandler() {
	}

	void AddNotification(const char* text, int severity, int lifetime)
	{
		notifications->push_front(new Notification(text, severity, lifetime));
	}


	void Draw(bool isImGuiActive)
	{
		long long currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

		float previousWindowHeight = 5.0f;
		if (isImGuiActive) // menu visible
			previousWindowHeight += 25.0f;

		for (size_t i = 0; i < notifications->size(); i++)
		{
			Notification* notification = notifications->at(i);
			std::string id("##");
			id.append(std::to_string(i));

			// handle position
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			center.y = previousWindowHeight;
			ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.0f));

			ImVec4 textColor = severityColors[notification->_severity];

			// fadeout animation
			long long animStartTime = notification->_deletionTime - animationTime;
			float alpha = 1.0f - ((float)(currentTime - animStartTime)) / animationTime;
			if (animStartTime < currentTime) {
				ImGui::SetNextWindowBgAlpha(alpha);
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0)); // remove border instantly. otherwise it looks weird on fadeout
				textColor.w = alpha;
			}

			// window element
			if (ImGui::Begin(id.c_str(), &enabled, windowFlags)) {
				ImGui::SetWindowSize(ImVec2(0, 0), ImGuiCond_Always);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4); // move text up a bit  to center it correctly in window
				ImGui::TextColored(textColor, severityIcons[notification->_severity]);
				ImGui::SameLine(0, 10);
				ImGui::TextColored(textColor, notification->_text.c_str());

				previousWindowHeight += ImGui::GetWindowSize().y + 3.0f;
			}
			ImGui::End(); // close window element

			if (animStartTime < currentTime) {
				ImGui::PopStyleColor();
			}

			// remove if time run out
			if (notification->_deletionTime < currentTime)
			{
				notifications->erase(notifications->begin() + i);
				i--;
			}
		}
	}
};

extern NotificationHandler notificationHandler;
