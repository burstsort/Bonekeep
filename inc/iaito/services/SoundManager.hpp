#ifndef IAITO_SOUND_MANAGER_HPP
#define IAITO_SOUND_MANAGER_HPP

#include <queue>

#include "iaito/core/Service.hpp"
#include "iaito/utility/OpenAL.hpp"
#include "iaito/utility/OggVorbis.hpp"

#include "iaito/resources/Audio.hpp"
#include "iaito/core/ResourceManager.hpp"

namespace iaito
{

	#define MAX_SOUNDS 16
	#define BUFFER_COUNT 16
	#define BUFFER_SIZE 4096
	
	#define audioIndex unsigned int
	#define soundIndex unsigned int

	struct Sound
	{
		unsigned long id;
		audioIndex audio;
		
		ALuint source;
		ALuint buffers[BUFFER_COUNT];

		ogg_file oggPtr;
		OggVorbis_File oggFile;

		ALenum format;
		ALsizei freq;

		bool pause;
		bool loop;
	};

	class SoundManager: public Service
	{
		public:
			SoundManager();
			~SoundManager();

			audioIndex BindAudio(const std::string& filename);
			void UnbindAudio(audioIndex audio);
			
			void ReloadAudio();
			void ClearAudio();

			soundIndex Play(audioIndex audio);
			soundIndex Loop(audioIndex audio);

			void Pause(soundIndex index);
			void Resume(soundIndex index);
			void Stop(soundIndex index);

			void PauseAll();
			void ResumeAll();
			void StopAll();

			void Update();

		private:
			SoundManager(const SoundManager& that);
			SoundManager& operator=(const SoundManager& that);

			ResourceManager<Audio> _audioMgr;

			ALCdevice* _device;
			ALCcontext* _context;

			Sound _sounds[MAX_SOUNDS];
			char _data[BUFFER_SIZE];

			unsigned long _nextId;
	};
}

#endif // IAITO_SOUND_MANAGER_HPP