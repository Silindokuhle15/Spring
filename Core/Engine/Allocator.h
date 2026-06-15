#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_
#include <vector>

template<typename T>
class ArenaAllocator
{
private:
	std::vector<void*> m_Blocks;
	void* m_CurrentBlock = nullptr;
	size_t m_Capacity;
	size_t m_Size;

public:
	explicit ArenaAllocator(size_t initial_capacity = 65535 * 2) :
		m_Capacity{ initial_capacity },
		m_Size{ 0 }
	{
		allocate_block(m_Capacity);
	}
	~ArenaAllocator()
	{
		for (void* block : m_Blocks)
		{
			std::free(block);
		}
	}

	ArenaAllocator(const ArenaAllocator&) = delete;
	ArenaAllocator& operator=(const ArenaAllocator&) = delete;

	template<typename... Args>
	T* allocate(Args&&... args)
	{
		if (m_Size >= m_Capacity)
		{
			grow();
		}

		void* addr = static_cast<T*>(m_CurrentBlock) + m_Size;
		T* obj = new(addr)T(std::forward<Args>(args)...);
		++m_Size;
		return obj;
	}

	void reset()
	{
		m_Size = 0;
		m_CurrentBlock = m_Blocks.front();
	}

private:
	void allocate_block(size_t capacity)
	{
		void* block = std::malloc(sizeof(T) * capacity);
		assert(block && "ArenaAllocator: Allocation failed");

		m_Blocks.push_back(block);
		m_CurrentBlock = block;
	}

	void grow()
	{
		m_Capacity *= 2;
		allocate_block(m_Capacity);
		m_Size = 0;
	}
};


template<typename T>
class BlockAllocator
{
private:
	struct Block
	{
		size_t capacity;
		T* data;
	};
public:
	explicit BlockAllocator(size_t block_size = 65535 * 16):
		m_CurrentBlockOffset{0}
	{
		allocate_block(block_size);
	}

	~BlockAllocator()
	{
		for (auto& block : m_Blocks)
		{
			std::free(block.data);
		}
	}

	BlockAllocator(const BlockAllocator&) = delete;
	BlockAllocator& operator=(const BlockAllocator&) = delete;

	T* allocate(size_t capacity)
	{
		if (capacity + m_CurrentBlockOffset > m_CurrentBlock.capacity)
		{
			size_t tCap = m_CurrentBlock.capacity * 2;
			if (tCap < capacity)
			{
				allocate_block(capacity);
			}
			else {
				allocate_block(tCap);
			}
			m_CurrentBlockOffset = 0;
		}
		T* first = static_cast<T*>(m_CurrentBlock.data) + m_CurrentBlockOffset;
		m_CurrentBlockOffset += capacity;
		return first;
	}

	void reset()
	{
		m_CurrentBlockOffset = 0;
		m_CurrentBlock = m_Blocks.front();
	}
private:
	void allocate_block(size_t capacity)
	{
		Block block{};
		block.capacity = capacity;
		block.data = static_cast<T*>(std::malloc(sizeof(T) * capacity));
		assert(block.data && "BlockAllocator: Allocation failed");
		m_Blocks.push_back(block);
		m_CurrentBlock = block;
	}

private:
	std::vector<Block> m_Blocks;
	Block m_CurrentBlock;
	size_t m_CurrentBlockOffset;
};
#endif