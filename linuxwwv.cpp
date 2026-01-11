#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>
#include <cstdlib>

constexpr int AUDIO_LATENCY_MS = 64; // tune as needed
using sys_clock = std::chrono::system_clock;

// ---------- helpers ----------

std::tm utc_now_tm()
{
    std::time_t now = std::time(nullptr);
    std::tm out{};
    gmtime_r(&now, &out);
    return out;
}

sys_clock::time_point next_minute_tp()
{
    auto now = sys_clock::now();
    auto sec = std::chrono::time_point_cast<std::chrono::seconds>(now);
    return sec + std::chrono::minutes(1);
}

void sleep_until_compensated(sys_clock::time_point target)
{
    auto fire = target - std::chrono::milliseconds(AUDIO_LATENCY_MS);
    std::this_thread::sleep_until(fire);
}

void play_cmd(const std::string& cmd)
{
    std::system(cmd.c_str());
}

// ---------- main ----------

int main()
{
    std::string snd = "./sounds/";

    // speech
    std::string pre_file    = snd + "wwv_at_the_tone.mp3";
    std::string post_file   = snd + "wwv_utc.mp3";
    std::string hr_file     = snd + "hour.mp3";
    std::string hrs_file    = snd + "hours.mp3";
    std::string mn_file     = snd + "minute.mp3";
    std::string mns_file    = snd + "minutes.mp3";

    while (true)
    {
		// 1) Wait for the next exact minute
		auto now = sys_clock::now();
		auto next_minute =
		std::chrono::time_point_cast<std::chrono::minutes>(now)
		+ std::chrono::minutes(1);
		std::this_thread::sleep_until(next_minute);

		// 2) Get UTC time *at the boundary*
		std::time_t tt = sys_clock::to_time_t(next_minute);
		std::tm utc{};
		gmtime_r(&tt, &utc);

		int hour   = utc.tm_hour;
		int minute = utc.tm_min;
		int speak_hour = hour;
		int speak_minute = minute + 1;

		if (minute == 60)
			{
				minute = 0;
				hour = (hour + 1) % 24;
			}
		if (speak_minute == 60)
			{  
				speak_minute = 0;
				speak_hour = (hour + 1) % 24;
			}

		// 3) Play tone (compensated)
		std::this_thread::sleep_until(
		next_minute - std::chrono::milliseconds(AUDIO_LATENCY_MS));

		if (minute == 0)
			system("aplay -q sounds/wwv-style-time-signal-toth.wav &");
		else if (minute >= 43 && minute <= 52)
			system("aplay -q sounds/wwv-style-time-signal-default.wav &");
		else if (minute % 2 == 0)
			{
				if (minute == 2)
					system("aplay -q sounds/wwv-style-time-signal-440.wav &");
				else if (minute == 4 || minute == 8 || minute == 10 || minute == 18 || minute == 30)
					system("aplay -q sounds/wwv-style-time-signal-default.wav &");
				else
					system("aplay -q sounds/wwv-style-time-signal-500.wav &");
			}
		else if (minute % 2 == 1)
			{
				if (minute == 3 || minute == 29 || minute == 59)
					system("aplay -q sounds/wwv-style-time-signal-default.wav &");
				else
					system("aplay -q sounds/wwv-style-time-signal-600.wav &");
			}

		// 4) Station ID
		if (minute == 0 || minute == 30)
			{
			std::this_thread::sleep_until(
			next_minute + std::chrono::seconds(2)
			- std::chrono::milliseconds(AUDIO_LATENCY_MS));
			system("mpg123 -q sounds/my-station-id.mp3");
			}

        // ---- build spoken time string ----

        auto build_number = [&](int n)
			{
				if (n <= 20)
					return snd + std::to_string(n) + ".mp3";
				int tens = n / 10;
				int ones = n % 10;
				if (ones == 0)
					return snd + std::to_string(tens) + "0.mp3";
				return snd + std::to_string(tens) + "0.mp3 " + snd + std::to_string(ones) + ".mp3";
			};

			std::string hr_snd = build_number(speak_hour);
			std::string mn_snd = build_number(speak_minute);
			std::string hrs_s = (speak_hour <= 1) ? hr_file : hrs_file;
			std::string mns_s = (speak_minute <= 1) ? mn_file : mns_file;

			std::string speak_time =
				pre_file + " " +
				hr_snd + " " +
				hrs_s + " " +
				mn_snd + " " +
				mns_s + " " +
				post_file;

			// :50— spoken time every minute
			std::this_thread::sleep_until(
			next_minute + std::chrono::seconds(50)
			- std::chrono::milliseconds(AUDIO_LATENCY_MS));
			play_cmd("mpg123 -q " + speak_time);
    }
}
