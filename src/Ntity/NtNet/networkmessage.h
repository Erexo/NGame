#pragma once

namespace Ntity
{
	namespace NtNet
	{
		class NetworkMessage
		{
			using message_size = uint16_t;

		public:
			static constexpr message_size MAX_SIZE = 24590;
			static constexpr int8_t HEADER_SIZE = sizeof(message_size);

			inline message_size getPosition() const { return position; }
			inline message_size getLength() const { return data.length; }
			inline void reset() { position = 0; }

			inline uint8_t* getBuffer() const { return (uint8_t*)& data; }
			inline uint8_t* getContentBuffer() const { return getBuffer() + HEADER_SIZE; }

			uint8_t getByte();
			std::string getString();
			template <typename T>
			T get();

			bool addByte(uint8_t value);
			template<typename T>
			bool add(T value);
			bool addString(const std::string& value);

		private:
			struct MessageData
			{
				message_size length = 0;
				uint8_t buffer[MAX_SIZE];
			};

			MessageData data;
			message_size position = 0;

			inline bool canAdd(size_t size)
			{
				return position + size < MAX_SIZE;
			}

			inline bool canRead(size_t size)
			{
				return position + size <= data.length;
			}
		};

		template <typename T>
		T Ntity::NtNet::NetworkMessage::get()
		{
			if (!canRead(sizeof(T)))
				return 0;

			T ret;
			memcpy(&ret, data.buffer + position, sizeof(T));
			position += sizeof(T);
			return ret;
		}

		template<typename T>
		bool Ntity::NtNet::NetworkMessage::add(T value)
		{
			if (!canAdd(sizeof(T)))
				return false;

			memcpy(data.buffer + position, &value, sizeof(T));
			position += sizeof(T);
			data.length += sizeof(T);
			return true;
		}
	}
}