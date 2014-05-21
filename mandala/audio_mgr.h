#pragma once

//mandala
#include "types.h"

//std
#include <memory>
#include <mutex>
#include <vector>

//al
#include <AL\alc.h>

namespace mandala
{
	struct sound_t;

	struct audio_mgr_t
	{
		struct device_t
		{
			device_t();
			~device_t();

			ALCdevice* ptr = nullptr;

		private:
			device_t(const device_t&) = delete;
			device_t& operator=(const device_t&) = delete;
		};

		struct context_t
		{
			context_t(const std::shared_ptr<device_t>& device);
			~context_t();

			ALCcontext* ptr = nullptr;

		private:
			context_t(const context_t&) = delete;
			context_t& operator=(const context_t&) = delete;
		};

		struct source_t
		{
			enum class state_t
			{
				initial,
				playing,
				paused,
				stopped
			};

			source_t();
			~source_t();

			state_t get_state() const;
			
			void play();
			void pause();
			void rewind();
			void stop();
			void queue_buffer(uint32_t buffer);
			void unqueue_buffer(uint32_t buffer);

			uint32_t id;

		private:
			source_t(const source_t&) = delete;
			source_t& operator=(const source_t&) = delete;
		};

		audio_mgr_t();
		~audio_mgr_t();

		void tick(float32_t dt);

		void get_devices();
		std::shared_ptr<source_t> create_source();
		std::shared_ptr<source_t> play(std::shared_ptr<sound_t> sound);

		std::vector<std::shared_ptr<source_t>> sources;

	private:
		std::shared_ptr<device_t> device;
		std::shared_ptr<context_t> context;
		std::mutex mutex;
	};
};