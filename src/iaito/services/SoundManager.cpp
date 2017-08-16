#include "iaito/services/SoundManager.hpp"

namespace iaito
{
	SoundManager::SoundManager() : _nextId(0)
	{
		// Create OpenAL context
		_device = alcOpenDevice(NULL);
		_context = alcCreateContext(_device, NULL);
		alcMakeContextCurrent(_context);

		// Set up listener
		ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

		alCheck(alListener3f(AL_POSITION, 0, 0, 1.0f));
		alCheck(alListener3f(AL_VELOCITY, 0, 0, 0));
		alCheck(alListenerfv(AL_ORIENTATION, listenerOri));

		// Create audio objects
		for (unsigned int i = 0; i < MAX_SOUNDS; ++i)
		{
			_sounds[i].id = 0;

			alCheck(alGenSources(1, &_sounds[i].source));

			for (unsigned int j = 0; j < BUFFER_COUNT; ++j)
			{
				alCheck(alGenBuffers(1, &_sounds[i].buffers[j]));
			}
		}
	}

	SoundManager::~SoundManager()
	{
		StopAll();

		// Stop sounds and destroy audio objects
		for (unsigned int i = 0; i < MAX_SOUNDS; ++i)
		{
			alCheck(alDeleteSources(1, &_sounds[i].source));

			for (unsigned int j = 0; j < BUFFER_COUNT; ++j)
			{
				alCheck(alDeleteBuffers(1, &_sounds[i].buffers[j]));
			}
		}

		// Destroy OpenAL context
		alcMakeContextCurrent(NULL);
		alcDestroyContext(_context);
		alcCloseDevice(_device);
	}

	audioIndex SoundManager::BindAudio(const std::string& filename)
	{
		return _audioMgr.Bind(filename);
	}

	void SoundManager::UnbindAudio(audioIndex audio)
	{
		for (unsigned int i = 0; i < MAX_SOUNDS; ++i)
		{
			if (_sounds[i].audio == audio)
			Stop(i);
		}

		_audioMgr.Unbind(audio);
	}

	void SoundManager::ReloadAudio()
	{
		StopAll();
		_audioMgr.Reload();
	}

	void SoundManager::ClearAudio()
	{
		StopAll();
		_audioMgr.Clear();
	}

	soundIndex SoundManager::Play(audioIndex audio)
	{
		if (_audioMgr.Size() <= audio)
			return -1;

		// Look for free sound objects
		unsigned int index = -1;
		for (unsigned int i = 0; i < MAX_SOUNDS; ++i)
		{
			if (!_sounds[i].id)
			{
				index = i;
				break;
			}
		}

		// Look for non-looping sound object to replace it
		if (index == (unsigned int)-1)
		{
			unsigned int min = (unsigned int)-1;
			for (unsigned int i = 0; i < MAX_SOUNDS; ++i)
			{
				if (_sounds[i].id < min && !_sounds[i].loop)
				{
					min = _sounds[i].id;
					index = i;
				}
			}
			Stop(index);
		}

		// Return -1 if no sound objects found
		if (index == (unsigned int)-1)
			return -1;

		// Store sound id
		_sounds[index].id = ++_nextId;

		// Store audio index
		_sounds[index].audio = audio;

		// Set up file pointer
		_sounds[index].oggPtr.curPtr = _sounds[index].oggPtr.filePtr = _audioMgr.Get(audio).GetData();
		_sounds[index].oggPtr.fileSize = _audioMgr.Get(audio).GetSize();

		// Start decoding the ogg vorbis file
		ov_callbacks callbacks = {readOgg, seekOgg, closeOgg, tellOgg};
		ov_open_callbacks((void*)&_sounds[index].oggPtr, &_sounds[index].oggFile, NULL, 0, callbacks);
		vorbis_info* pInfo = ov_info(&_sounds[index].oggFile, -1);

		// Store format information
		if (pInfo->channels == 1)
			_sounds[index].format = AL_FORMAT_MONO16;
		else if (pInfo->channels == 2)
			_sounds[index].format = AL_FORMAT_STEREO16;

		_sounds[index].freq = pInfo->rate;

		_sounds[index].pause = false;
		_sounds[index].loop = false;

		// Preload and queue buffers
		for (unsigned int i = 0; i < BUFFER_COUNT; ++i)
		{
			int bitStream;
			unsigned int bytes = ov_read(&_sounds[index].oggFile, _data, BUFFER_SIZE, 0, 2, 1, &bitStream);
			
			if (bytes)
			{
				alCheck(alBufferData(_sounds[index].buffers[i], _sounds[index].format, _data, bytes, _sounds[index].freq));
				alCheck(alSourceQueueBuffers(_sounds[index].source, 1, &_sounds[index].buffers[i]));
			}
		}

		// Start playing
		alCheck(alSourcePlay(_sounds[index].source));

		return index;
	}

	soundIndex SoundManager::Loop(audioIndex audio)
	{
		soundIndex index = Play(audio);

		if (index >= MAX_SOUNDS)
			return -1;

		_sounds[index].loop = true;
		return index;
	}

	void SoundManager::Pause(soundIndex index)
	{
		// Check if index valid
		if (index >= MAX_SOUNDS)
			return;

		// Check if audio object enabled or already paused
		if (!_sounds[index].id || _sounds[index].pause)
			return;

		_sounds[index].pause = true;
		alCheck(alSourcePause(_sounds[index].source));
	}


	void SoundManager::Resume(soundIndex index)
	{
		// Check if index valid
		if (index >= MAX_SOUNDS)
			return;

		// Check if audio object enabled or already playing
		if (!_sounds[index].id || !_sounds[index].pause)
			return;

		_sounds[index].pause = false;
		alCheck(alSourcePlay(_sounds[index].source));
	}


	void SoundManager::Stop(soundIndex index)
	{
		// Check if index valid
		if (index >= MAX_SOUNDS)
			return;

		// Check if disabled
		if (!_sounds[index].id)
			return;

		// Stop playing
		alCheck(alSourceStop(_sounds[index].source));

		// Unqueue buffers
		ALint queued = 0;
		alCheck(alGetSourcei(_sounds[index].source, AL_BUFFERS_QUEUED, &queued));
		for (int i = 0; i < queued; ++i)
		{
			ALuint buffer;
			alCheck(alSourceUnqueueBuffers(_sounds[index].source, 1, &buffer));
		}

		// Stop decoding
		ov_clear(&_sounds[index].oggFile);

		// Disable audio object
		_sounds[index].id = 0;
	}

	void SoundManager::PauseAll()
	{
		for (unsigned int i = 0; i < MAX_SOUNDS; ++i)
		{
			Pause(i);
		}
	}

	void SoundManager::ResumeAll()
	{
		for (unsigned int i = 0; i < MAX_SOUNDS; ++i)
		{
			Resume(i);
		}
	}

	void SoundManager::StopAll()
	{
		for (unsigned int i = 0; i < MAX_SOUNDS; ++i)
		{
			Stop(i);
		}
	}

	void SoundManager::Update()
	{
		for (unsigned int i = 0; i < MAX_SOUNDS; ++i)
		{
			// Check if audio object disabled or paused
			if (!_sounds[i].id || _sounds[i].pause)
				continue;

			// Get the number of processed buffers
			ALint processed = 0;
			alCheck(alGetSourcei(_sounds[i].source, AL_BUFFERS_PROCESSED, &processed));
			
			while (processed--)
			{
				ALuint buffer;
				alCheck(alSourceUnqueueBuffers(_sounds[i].source, 1, &buffer));

				// Read data
				int bitStream;
				unsigned int bytes = ov_read(&_sounds[i].oggFile, _data, BUFFER_SIZE, 0, 2, 1, &bitStream);

				// If no bytes were read
				if (!bytes)
				{
					if (_sounds[i].loop)
					{
						// Start from beginning
						ov_time_seek(&_sounds[i].oggFile, 0);
						bytes = ov_read(&_sounds[i].oggFile, _data, BUFFER_SIZE, 0, 2, 1, &bitStream);
					}
					else
					{
						// Stop playing
						Stop(i);
						break;
					}
				}

				// Update and queue the buffers
				if (bytes)
				{
					alCheck(alBufferData(buffer, _sounds[i].format, _data, bytes, _sounds[i].freq));
					alCheck(alSourceQueueBuffers(_sounds[i].source, 1, &buffer));
				}
			}

			// Resume playing if stopped
			ALint state;
			alCheck(alGetSourcei(_sounds[i].source, AL_SOURCE_STATE, &state));
			if (_sounds[i].id && state == AL_STOPPED)
				alCheck(alSourcePlay(_sounds[i].source));
		}
	}
}
