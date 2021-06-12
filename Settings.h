#pragma once

enum class TimeStepType { Fixed, Variable };

using SettingType = std::variant<int, float, bool, TimeStepType, std::string>;

class Settings
{
public:

	class StandardSettings {
	public:

		StandardSettings() {
			setSetting("Title", std::string("PlatformerPort"));
			//setSetting("Width", 1280);
			//setSetting("Height", 960);

			setSetting("Width", 1920);
			setSetting("Height", 1080);

			setSetting("targetFPS", 60.0f);
			setSetting("targetDT", 1.0f / 60.0f);
			setSetting("debug", false);
			setSetting("paused", false);
			setSetting("vsync", false);
			setSetting("running", true);
			setSetting("TimeStepType", TimeStepType::Fixed);
		}

		SettingType getSetting(const std::string& settingname) const {
			return settingsmap.at(settingname);
		}

		void setSetting(const std::string& settingName, const SettingType settingVal) {
			settingsmap.insert_or_assign(settingName, settingVal);
		}
	private:
		std::map<std::string, SettingType> settingsmap;
	};

	template<typename T>
	static T getSetting(const std::string& settingname) {
		static_assert(
			std::is_same<T, int>::value
			|| std::is_same<T, float>::value
			|| std::is_same<T, bool>::value
			|| std::is_same<T, TimeStepType>::value
			|| std::is_same<T, std::string>::value
			);

		if (!settings) {
			settings = std::make_unique<StandardSettings>();
		}
		//try {
		//	
		//}
		//catch (const std::bad_variant_access&) {
		//	return T();
		//}
		//catch (const std::out_of_range&) {
		//	return T();
		//}

		return std::get<T>(settings->getSetting(settingname));
	}

	template<typename T>
	static void setSetting(const std::string& settingName, const T settingVal) noexcept {
		static_assert(
			std::is_same<T, int>::value
			|| std::is_same<T, float>::value
			|| std::is_same<T, bool>::value
			|| std::is_same<T, TimeStepType>::value
			|| std::is_same<T, std::string>::value
			);

		if (!settings) {
			settings = std::make_unique<StandardSettings>();
		}
		settings->setSetting(settingName, settingVal);
	}
private:
	inline static std::unique_ptr<StandardSettings> settings;
};

